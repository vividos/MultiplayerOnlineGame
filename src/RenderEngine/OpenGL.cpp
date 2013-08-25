//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file OpenGL.cpp OpenGL helper functions
//

// includes
#include "StdAfx.h"
#include "OpenGL.hpp"
#include "Bitmap.hpp"
#include "Vector3d.hpp"

#pragma comment(lib, "glu32.lib")

bool OpenGL::IsExtensionSupported(LPCTSTR pszExtension)
{
   static std::map<CString, bool> s_mapExtensionSupported;

   // fill map
   if (s_mapExtensionSupported.empty())
   {
      const GLubyte* extension = glGetString(GL_EXTENSIONS);
      while(extension != NULL && *extension != 0)
      {
         const GLubyte* next = (GLubyte*)strchr((const char*)extension, ' ');
         if (next == NULL)
         {
            s_mapExtensionSupported.insert(std::make_pair(CString(extension), true));
         }
         else
         {
            s_mapExtensionSupported.insert(std::make_pair(CString((const char*)extension, int(next-extension)), true));
         }

         extension = next;

         while(extension != NULL && *extension != 0 && *extension == ' ')
            extension++;
      }
   }

   CString cszExtension(pszExtension);
   return s_mapExtensionSupported.find(cszExtension) != s_mapExtensionSupported.end();
}


/// global polygon (or triangle) count
static unsigned int g_uiPolycount = 0;

void OpenGL::CountPolygons(unsigned int uiPolycount)
{
   g_uiPolycount += uiPolycount;
}

void OpenGL::ResetPolycount()
{
   g_uiPolycount = 0;
}

unsigned int OpenGL::Polycount()
{
   return g_uiPolycount;
}

/// id of render thread
static DWORD g_dwRenderThreadId = 0;

bool OpenGL::IsRenderThread(DWORD dwThreadId)
{
   return g_dwRenderThreadId != 0 &&
      g_dwRenderThreadId == dwThreadId;
}

void OpenGL::SetRenderThreadId(DWORD dwThreadId)
{
   std::swap(g_dwRenderThreadId, dwThreadId);
}


void OpenGL::ReplaceTextureMipmapLevel(unsigned int texid,
   unsigned int uiLevel, unsigned int xres, unsigned int yres, Color color)
{
   glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texid));
   glTraceError("glBindTexture");

   Bitmap bmp(xres, yres, color);

   GLubyte* data = &bmp.Data()->m_color[0];

   glTexImage2D(GL_TEXTURE_2D, uiLevel, GL_RGBA, bmp.XRes(), bmp.YRes(), 0,
      GL_RGBA, GL_UNSIGNED_BYTE, data);

   glTraceError("glTexImage2D");
}

void OpenGL::RenderBoundingBox(const Vector3d& vMin, const Vector3d& vMax)
{
   glPushAttrib(GL_TEXTURE_BIT);
   glDisable(GL_TEXTURE_2D);

   // bottom
   glBegin(GL_LINE_LOOP);
      glVertex3d(vMin.X(), vMin.Y(), vMin.Z());
      glVertex3d(vMax.X(), vMin.Y(), vMin.Z());
      glVertex3d(vMax.X(), vMin.Y(), vMax.Z());
      glVertex3d(vMin.X(), vMin.Y(), vMax.Z());
   glEnd();

   // top
   glBegin(GL_LINE_LOOP);
      glVertex3d(vMin.X(), vMax.Y(), vMin.Z());
      glVertex3d(vMax.X(), vMax.Y(), vMin.Z());
      glVertex3d(vMax.X(), vMax.Y(), vMax.Z());
      glVertex3d(vMin.X(), vMax.Y(), vMax.Z());
   glEnd();

   //// lines connecting bottom and top rectangles
   glBegin(GL_LINES);
      glVertex3d(vMin.X(), vMin.Y(), vMin.Z());
      glVertex3d(vMin.X(), vMax.Y(), vMin.Z());

      glVertex3d(vMax.X(), vMin.Y(), vMin.Z());
      glVertex3d(vMax.X(), vMax.Y(), vMin.Z());

      glVertex3d(vMax.X(), vMin.Y(), vMax.Z());
      glVertex3d(vMax.X(), vMax.Y(), vMax.Z());

      glVertex3d(vMin.X(), vMin.Y(), vMax.Z());
      glVertex3d(vMin.X(), vMax.Y(), vMax.Z());
   glEnd();

   glPopAttrib();
}

void OpenGL::RenderXyzAxes()
{
   glPushAttrib(GL_TEXTURE_BIT | GL_LIGHTING_BIT | GL_LINE_BIT);

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);

   glLineWidth(3.0f);

   glBegin(GL_LINES);
   glColor3ub(255, 0, 0); glVertex3d(0.0, 0.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
   glColor3ub(0, 255, 0); glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 1.0, 0.0);
   glColor3ub(0, 0, 255); glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 1.0);
   glEnd();

   glPopAttrib();
   glColor3ub(255, 255, 255); 
}
