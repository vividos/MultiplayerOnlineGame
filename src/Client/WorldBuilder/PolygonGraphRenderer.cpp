//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PolygonGraphRenderer.cpp Render class for polygon graph
//

// include
#include "StdAfx.h"
#include "PolygonGraphRenderer.hpp"
#include "PolygonGraph.hpp"
#include "Vector3d.hpp"
#include <boost/foreach.hpp>
#include <SDL_opengl.h>

/// scale factor for elevation (that is between 0.0 and 1.0)
const double c_dElevationScaleFactor = 256.0;

PolygonGraphRenderer::PolygonGraphRenderer(const PolygonGraph::Graph& graph) throw()
:m_graph(graph),
 m_bUploadDone(false)
{
}

void PolygonGraphRenderer::Upload()
{
   m_bUploadDone = false;

   PrepareEdges();
   PreparePolygons();

   m_bUploadDone = true;
}

void PolygonGraphRenderer::PrepareEdges()
{
   m_displayListEdges.Init();

   m_displayListEdges.Open();

   glLineWidth(4.0);

   glBegin(GL_LINES);

   BOOST_FOREACH(PolygonGraph::EdgePtr spEdge, m_graph.m_edges)
   {
      PolygonGraph::CornerPtr v0 = spEdge->v0;
      PolygonGraph::CornerPtr v1 = spEdge->v1;

      if (spEdge->river > 0)
         glColor3ub(113,116,242);
      else
         glColor3ub(255,255,255);

      glVertex3d(v0->point.X(), v0->elevation * c_dElevationScaleFactor, v0->point.Y());
      glVertex3d(v1->point.X(), v1->elevation * c_dElevationScaleFactor, v1->point.Y());
   }

   glEnd();

   m_displayListEdges.Close();
}

void PolygonGraphRenderer::PreparePolygons()
{
   m_displayListPolygons.Init();

   m_displayListPolygons.Open();

   BOOST_FOREACH(PolygonGraph::CenterPtr p, m_graph.m_centers)
   {
      std::vector<PolygonGraph::CornerPtr> vecCorners = p->corners;

      // order them clockwise
      std::sort(vecCorners.begin(), vecCorners.end(),
         PolygonGraph::CornerSorter(p->point));

      // draw triangle fan
      glBegin(GL_TRIANGLE_FAN);

      Color c = PolygonGraph::ColorByBiomeType(p->enBiomeType);
      //Color c = PolygonGraph::ColorByTerrainType(p->enTerrainType);
      //Color c = PolygonGraph::ColorByElevation(p->elevation);
      //Color c = PolygonGraph::ColorByMoisture(p->moisture);


      glColor3ubv(c.m_color);

      // normal
      ATLASSERT(vecCorners.size() >= 2);
      Vector3d center(p->point.X(), p->elevation * c_dElevationScaleFactor, p->point.Y());
      Vector3d p1(vecCorners[0]->point.X(), vecCorners[0]->elevation * c_dElevationScaleFactor, vecCorners[0]->point.Y());
      Vector3d p2(vecCorners[1]->point.X(), vecCorners[1]->elevation * c_dElevationScaleFactor, vecCorners[1]->point.Y());
      Vector3d normal;
      normal.Cross(center-p1, center-p2);
      normal.Normalize();

      glNormal3dv(normal.Data());

      // center
      glVertex3d(p->point.X(), p->elevation * c_dElevationScaleFactor, p->point.Y());

      BOOST_FOREACH(PolygonGraph::CornerPtr q, vecCorners)
         glVertex3d(q->point.X(), q->elevation * c_dElevationScaleFactor, q->point.Y());

      glVertex3d(vecCorners[0]->point.X(), vecCorners[0]->elevation * c_dElevationScaleFactor,
         vecCorners[0]->point.Y());

      glEnd();
   }

   m_displayListPolygons.Close();
}

void PolygonGraphRenderer::Render(RenderOptions& /*options*/)
{
   if (!m_bUploadDone)
      return;

   m_displayListEdges.Call();
   m_displayListPolygons.Call();
}
