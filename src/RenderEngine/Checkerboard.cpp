//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Checkerboard.cpp Checkerboard render class
//

// includes
#include "StdAfx.h"
#include "Checkerboard.hpp"
#include "OpenGL.hpp"

void Checkerboard::Render(RenderOptions& /*options*/) throw()
{
   glDisable(GL_TEXTURE_2D);

   glColor3f(1.0f, 1.0f, 1.0f);

   // render checkerboard
   glBegin(GL_QUADS);

   for(int x=m_xMin; x<m_xMax; x++)
   for(int y=m_yMin; y<m_yMax; y++)
   {
      if ((x + y) & 1)
         glColor3f(1.0f, 1.0f, 1.0f);
      else
         glColor3f(0.2f, 0.2f, 0.2f);
      glVertex3i(x,   int(m_fHeight), y);
      glVertex3i(x,   int(m_fHeight), y+1);
      glVertex3i(x+1, int(m_fHeight), y+1);
      glVertex3i(x+1, int(m_fHeight), y);
   }
   glEnd();

   OpenGL::CountPolygons((m_xMax-m_xMin)*(m_yMax-m_yMin)*2);
}
