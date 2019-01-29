//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file OpenGL.hpp OpenGL helper functions
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include <SDL_OpenGL.h>
#include <SDL_opengl_glext.h>
#include <gl/glu.h>
#include "Color.hpp"
#include "Vector3.hpp"

/// shared pointer for Texture
typedef std::shared_ptr<class Texture> TexturePtr;

// auto-link
#pragma comment(lib, "opengl32.lib")

/// helper macro to output OpenGL errors when previous OpenGL call failed
#ifdef _DEBUG
   #define glTraceError(x) \
      { \
         GLenum err = ::glGetError(); \
         if (err != GL_NO_ERROR) \
            ATLTRACE(_T("%hs(%u): GL error calling %hs(): %hs\n"), __FILE__, __LINE__, x, \
               reinterpret_cast<const char*>(gluErrorString(err))); \
      }
#pragma comment(lib, "glu32.lib")
#else
   // in release, we don't call glGetError() to not stall the render pipeline
   #define glTraceError(x)
#endif

/// \brief OpenGL helper functions and classes
namespace OpenGL
{
   /// OpenGL extension strings
   namespace Extension
   {
      /// extension to support non-power-of-two texture sizes
      static TCHAR ARB_texture_non_power_of_two[] = _T("GL_ARB_texture_non_power_of_two");

      /// extension to support VBO
      static TCHAR ARB_vertex_buffer_object[] = _T("GL_ARB_vertex_buffer_object");

      /// extension to support framebuffer object
      static TCHAR ARB_framebuffer_object[] = _T("GL_ARB_framebuffer_object");

      /// extension to support glDrawBuffers call
      static TCHAR ARB_draw_buffers[] = _T("GL_ARB_draw_buffers");
   }

   /// returns if a given extension is supported
   bool RENDERENGINE_DECLSPEC IsExtensionSupported(LPCTSTR pszExtension);

   /// logs OpenGL diagnostics
   void RENDERENGINE_DECLSPEC LogDiagnostics();


   // polycount

   /// counts polygons
   void RENDERENGINE_DECLSPEC CountPolygons(unsigned int uiPolycount);

   /// resets polygon count
   void RENDERENGINE_DECLSPEC ResetPolycount();

   /// returns current polygon count
   unsigned int RENDERENGINE_DECLSPEC Polycount();


   // render context check

   /// returns if the current (or given) thread is the thread with render context
   bool RENDERENGINE_DECLSPEC IsRenderThread(DWORD dwThreadId = ::GetCurrentThreadId());

   /// sets thread id of thread with render context; only used by RenderEngine
   void RENDERENGINE_DECLSPEC SetRenderThreadId(DWORD dwThreadId = ::GetCurrentThreadId());


   // render helper functions

   /// replace textures at all mipmap levels with colors
   void RENDERENGINE_DECLSPEC ReplaceTextureAllMipmapLevels(TexturePtr spTexture, unsigned int xres, unsigned int yres);

   /// replaces texture at given mipmap level with given color
   void RENDERENGINE_DECLSPEC ReplaceTextureMipmapLevel(unsigned int texid,
      unsigned int uiLevel, unsigned int xres, unsigned int yres, Color color);

   /// renders axis aligned bounding box by given min and max points
   void RENDERENGINE_DECLSPEC RenderBoundingBox(const Vector3d& vMin, const Vector3d& vMax);

   /// renders 3 colored axes, each one unit long; x axis: red, y axis: green, z axis: blue
   void RENDERENGINE_DECLSPEC RenderXyzAxes();

   /// renders a sphoere at given position
   void RenderSphere(const Vector3d& vPos, double dRadius);

   /// class to push attributes on attribute stack
   class PushedAttributes
   {
   public:
      /// ctor
      /// \param mask bits of attributes to push to attribute stack
      /// Here's an incomplete list of bits and what attributes they push:
      /// GL_CURRENT_BIT: current RGBA color, normal vector, tex coords, ...
      /// GL_COLOR_BUFFER_BIT: GL_ALPHA_TEST enable bit, GL_BLEND enable bit, ...
      /// GL_DEPTH_BUFFER_BIT: GL_DEPTH_TEST enable bit, ...
      /// GL_ENABLE_BIT: all flags
      /// GL_LINE_BIT: Line width, GL_LINE_STIPPLE enable bit, ...
      /// GL_POLYGON_BIT: GL_CULL_FACE enable bit, GL_CULL_FACE_MODE value, ...
      PushedAttributes(GLbitfield mask)
      {
         glPushAttrib(mask);
#ifdef _DEBUG
         GLenum err = glGetError();
         ATLASSERT(GL_STACK_OVERFLOW != err); // attribute stack nested too deep!
#endif
      }
      /// dtor
      ~PushedAttributes()
      {
         glPopAttrib();
      }
   };

   /// class to restore viewport in dtor
   class ViewportRestorer
   {
   public:
      /// ctor; stores current viewport
      ViewportRestorer() throw()
      {
         glGetIntegerv(GL_VIEWPORT, &m_aiViewportInfo.front());
         glTraceError("glGetIntegerv");
      }
      /// dtor; restores viewport
      ~ViewportRestorer() throw()
      {
         glViewport(
            m_aiViewportInfo[0],
            m_aiViewportInfo[1],
            m_aiViewportInfo[2],
            m_aiViewportInfo[3]);
         glTraceError("glViewport");
      }

   private:
      /// stored viewport infos
      std::array<GLint, 4> m_aiViewportInfo;
   };

} // namespace OpenGL
