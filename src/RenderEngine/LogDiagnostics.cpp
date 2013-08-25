//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file LogDiagnostics.cpp OpenGL log diagnostics function
//

// includes
#include "StdAfx.h"
#include "OpenGL.hpp"

void OpenGL::LogDiagnostics()
{
   {
      GLint redbits, greenbits, bluebits, alphabits, depthbits;
      glGetIntegerv(GL_RED_BITS, &redbits);
      glGetIntegerv(GL_GREEN_BITS, &greenbits);
      glGetIntegerv(GL_BLUE_BITS, &bluebits);
      glGetIntegerv(GL_ALPHA_BITS, &alphabits);
      glGetIntegerv(GL_DEPTH_BITS, &depthbits);

      LOG_INFO(_T("OpenGL stats:"), Log::Client::OpenGL);

      CString cszText;
      cszText.Format(_T(" bit depths: red/green/blue/alpha = %u/%u/%u/%u, depth=%u"),
         redbits, greenbits, bluebits, alphabits, depthbits);

      LOG_INFO(cszText, Log::Client::OpenGL);
   }

   {
      GLint maxtexsize, maxlights, maxnamestack, maxmodelstack, maxprojstack;
      glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxtexsize);
      glGetIntegerv(GL_MAX_LIGHTS, &maxlights);
      glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &maxnamestack);
      glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &maxmodelstack);
      glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &maxprojstack);

      CString cszText;
      cszText.Format(_T(" max. texture size = %u x %u, max. lights = %u"),
         maxtexsize, maxtexsize, maxlights);

      LOG_INFO(cszText, Log::Client::OpenGL);

      cszText.Format(_T(" stack depths: name stack = %u, modelview stack = %u, proj. stack = %u"),
         maxnamestack, maxmodelstack, maxprojstack);

      LOG_INFO(cszText, Log::Client::OpenGL);
   }

   {
      GLint maxElementsIndices, maxElementsVertices;
      glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxElementsIndices);
      glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxElementsVertices);

      CString cszText;
      cszText.Format(_T(" max buffer elements: indices = %i, vertices = %i"),
         maxElementsIndices, maxElementsVertices);

      LOG_INFO(cszText, Log::Client::OpenGL);
   }

   {
      const GLubyte* vendor,* renderer,* version;
      vendor = glGetString(GL_VENDOR);
      renderer = glGetString(GL_RENDERER);
      version = glGetString(GL_VERSION);

      CString cszText;
      cszText.Format(_T("  version: %hs, renderer: %hs, vendor: %hs"),
         version, renderer, vendor);

      LOG_INFO(cszText, Log::Client::OpenGL);
   }

   {
      GLboolean stereo;
      glGetBooleanv(GL_STEREO, &stereo);

      CString cszText;
      cszText.Format(_T(" supports stereo mode: %s"),
         stereo == GL_TRUE ? _T("yes") : _T("no"));

      LOG_INFO(cszText, Log::Client::OpenGL);
   }

   // list all extensions
   {
      LOG_INFO(_T(" extensions:"), Log::Client::OpenGL);
      CString cszText;
      LPCSTR pszExtensions = reinterpret_cast<LPCSTR>(glGetString(GL_EXTENSIONS));
      for (; pszExtensions != NULL && pszExtensions[0] != 0;)
      {
         LPCSTR pszNext = strstr(pszExtensions, " ");

         if (pszNext != NULL)
            cszText.Format(_T("  %hs"), CStringA(pszExtensions, pszNext - pszExtensions));
         else
            cszText.Format(_T("  %hs"), pszExtensions);

         LOG_INFO(cszText, Log::Client::OpenGL);

         if (pszNext != NULL)
            pszNext++;

         pszExtensions = pszNext;
      }
   }
}
