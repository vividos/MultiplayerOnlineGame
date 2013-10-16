//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file OpenGL.hpp OpenGL helper functions
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include <SDL_OpenGL.h>
#include "Color.hpp"

// auto-link
#pragma comment(lib, "opengl32.lib")

// forward references
class Vector3d;

/// helper macro to output OpenGL errors when previous OpenGL call failed
#ifdef _DEBUG
#include <gl/glu.h>
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

   /// replaces texture at given mipmap level with given color
   void RENDERENGINE_DECLSPEC ReplaceTextureMipmapLevel(unsigned int texid,
      unsigned int uiLevel, unsigned int xres, unsigned int yres, Color color);

   /// renders axis aligned bounding box by given min and max points
   void RENDERENGINE_DECLSPEC RenderBoundingBox(const Vector3d& vMin, const Vector3d& vMax);

   /// renders 3 colored axes, each one unit long; x axis: red, y axis: green, z axis: blue
   void RENDERENGINE_DECLSPEC RenderXyzAxes();

} // namespace OpenGL
