//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file Voronoi2.cpp Voronoi diagram, using Boost.Polygon
//

// include
#include "StdAfx.h"
#include "Voronoi2.hpp"
#include <boost.voronoi/boost/polygon/voronoi_diagram.hpp>
#include <boost.voronoi/boost/polygon/voronoi_builder.hpp>
#include <boost.voronoi/boost/polygon/voronoi_utils.hpp>
#include "SVGOutput.hpp"

// moved here, from voronoi_ctypes.hpp, since it contains a constant that must be defined only once per project
namespace boost {
namespace polygon {
namespace detail {
const int extened_exponent_fpt_traits<fpt64>::kMaxSignificantExpDif = 54;
}
}
}

using namespace Voronoi2;

Diagram::Diagram(const std::vector<Vector2d>& vecPoints, const Vector2d& vBounds)
:m_vBounds(vBounds)
{
   boost::polygon::voronoi_builder<boost::int32_t> builder;

   for (size_t i=0, iMax=vecPoints.size(); i<iMax; i++)
   {
      const Vector2d& vPoint = vecPoints[i];
      builder.insert_point(static_cast<boost::int32_t>(vPoint.X()),
         static_cast<boost::int32_t>(vPoint.Y()));
   }

   builder.construct(&m_diagram);
}

size_t Diagram::NumSites() const
{
   return m_diagram.cells().size();
}

/// clips edge at boundary box and returns start and end point of edge
bool ClipEdge(const boost::polygon::voronoi_diagram<double>::edge_type& edge,
   const boost::polygon::bounding_rectangle<double>& bbox,
   Vector2d& start, Vector2d& end)
{
   if (edge.is_finite())
   {
      const boost::polygon::voronoi_vertex<double>* vv0 = edge.vertex0();
      const boost::polygon::voronoi_vertex<double>* vv1 = edge.vertex1();

      if (bbox.contains(vv0->vertex().x(), vv0->vertex().y()) &&
          bbox.contains(vv1->vertex().x(), vv1->vertex().y()))
      {
         start = Vector2d(vv0->vertex().x(), vv0->vertex().y());
         end =   Vector2d(vv1->vertex().x(), vv1->vertex().y());

         return true;
      }
   }

   // ray or line
   ATLASSERT(edge.is_linear());

   boost::polygon::voronoi_utils<double>::point_set_type polyline;
   boost::polygon::voronoi_utils<double>::clip(edge, bbox, polyline);

   if (polyline.size() != 2)
      return false; // completely clipped

   start = Vector2d(polyline[0].x(), polyline[0].y());
   end =   Vector2d(polyline[1].x(), polyline[1].y());

   if (start.X() < 0.0) start.X(0.0);
   if (start.Y() < 0.0) start.Y(0.0);

   if (end.X() < 0.0) end.X(0.0);
   if (end.Y() < 0.0) end.Y(0.0);

   return true;
}

void Diagram::GetAllEdges(std::vector<Edge>& vecAllEdges)
{
   boost::polygon::bounding_rectangle<double> bbox;
   bbox.update(0.0, 0.0);
   bbox.update(m_vBounds.X(), m_vBounds.Y());

   const std::vector<diagram_type::edge_type>& vecEdges = m_diagram.edges();

   for (size_t i=0, iMax=vecEdges.size(); i<iMax; i++)
   {
      const diagram_type::edge_type& edge = vecEdges[i];

      if (edge.is_primary())
      {
         Edge e;

         // get voronoi edge points
         if (!ClipEdge(edge, bbox, e.v0, e.v1))
            continue;

         // get delaunay triangulation points
         const diagram_type::point_type& d0 = edge.cell()->point0();
         const diagram_type::point_type& d1 = edge.twin()->cell()->point0();

         e.d0 = Vector2d(d0.x(), d0.y());
         e.d1 = Vector2d(d1.x(), d1.y());

         vecAllEdges.push_back(e);
      }
   }
}

void Diagram::Region(size_t index, std::vector<Vector2d>& vecRegionPoints)
{
   ATLASSERT(index < m_diagram.cells().size());

   const diagram_type::cell_type& cell = m_diagram.cells()[index];

   // we always have a point site cell
   ATLASSERT(cell.contains_point());

   if (cell.is_degenerate())
      return;

   boost::polygon::bounding_rectangle<double> bbox;
   bbox.update(0.0, 0.0);
   bbox.update(m_vBounds.X(), m_vBounds.Y());

   const diagram_type::edge_type* pFirstEdge = cell.incident_edge();
   const diagram_type::edge_type* pNextEdge = pFirstEdge;
   do
   {
      if (pNextEdge->is_primary())
      {
         Vector2d start, end;
         if (ClipEdge(*pNextEdge, bbox, start, end))
            vecRegionPoints.push_back(start);
      }

      pNextEdge = pNextEdge->next();
   }
   while (pNextEdge != pFirstEdge);
}

void Diagram::OutputSvg(LPCTSTR pszFilename, bool bDrawArrows, bool bDrawDelaunay)
{
   SVG::Writer writer(pszFilename, unsigned(m_vBounds.X()), unsigned(m_vBounds.Y()));

   {
      SVG::Style circleStyle;
      circleStyle.Add(_T("stroke"), _T("rgb(255,0,0)"));
      circleStyle.Add(_T("stroke-width"), _T("2"));

      // center circles
      for (size_t i=0, iMax=m_diagram.num_cells(); i<iMax; i++)
      {
         const boost::polygon::voronoi_cell<double>& cell = m_diagram.cells()[i];

         writer.OutputCircle(Vector2d(cell.point0().x(), cell.point0().y()), 5.0, circleStyle);
      }
   }

   SVG::Style edgeStyle;
   edgeStyle.Add(_T("stroke"), _T("rgb(0,0,255)"));
   edgeStyle.Add(_T("stroke-width"), _T("1.2"));

   SVG::Style delaunayStyle;
   delaunayStyle.Add(_T("stroke"), _T("rgb(0,255,0)"));
   delaunayStyle.Add(_T("stroke-width"), _T("2"));

   boost::polygon::bounding_rectangle<double> bbox;
   bbox.update(0.0, 0.0);
   bbox.update(m_vBounds.X(), m_vBounds.Y());

   // edges
   unsigned int uiCount = 0;
   for (size_t i=0, iMax=m_diagram.edges().size(); i<iMax; i++)
   {
      const diagram_type::edge_type& edge = m_diagram.edges()[i];

      if (!edge.is_linear())
         continue; // don't output arcs

      if (!edge.is_primary())
         continue; // don't output non-primary edges

      Vector2d start;
      Vector2d end;

      if (!ClipEdge(edge, bbox, start, end))
         continue; // completely clipped

      writer.OutputCircle(start, 2.0, edgeStyle);
      writer.OutputLine(start, end, edgeStyle);

      // paint arrow at the "end" side of segment
      if (bDrawArrows)
      {
         const double c_dArrowLen = 5.0;
         const double c_dArrowAngle = 155.0;

         Vector2d v1(end.X()-start.X(), end.Y()-start.Y());
         v1.Normalize();
         v1.Rotate(c_dArrowAngle);
         v1 *= c_dArrowLen;

         writer.OutputLine(end, end+v1, edgeStyle);

         Vector2d v2(end.X()-start.X(), end.Y()-start.Y());
         v2.Normalize();
         v2.Rotate(-c_dArrowAngle);
         v2 *= c_dArrowLen;

         writer.OutputLine(end, end+v2, edgeStyle);
      }

      uiCount++;

      // calc intersection of start/end and p0
      if (bDrawDelaunay)
      {
         // we always have a point site cell
         ATLASSERT(edge.cell()->contains_point());

         Vector2d p0(edge.cell()->point0().x(), edge.cell()->point0().y());

         Vector2d v0(p0.X()-start.X(), p0.Y()-start.Y()),
            v2(end.X()-start.X(), end.Y()-start.Y());

         Vector2d int1 = Vector2d(start.X(), start.Y()) + (v0.Dot(v2) / v2.Dot(v2)) * v2;

         if (p0.X() != 0.0 && p0.Y() != 0.0)
            writer.OutputLine(int1, p0, delaunayStyle);
      }

      // calc intersection of start/end and p1
      if (bDrawDelaunay)
      {
         Vector2d p1(edge.twin()->cell()->point0().x(), edge.twin()->cell()->point0().y());

         Vector2d v0(p1.X()-start.X(), p1.Y()-start.Y()),
            v2(end.X()-start.X(), end.Y()-start.Y());

         Vector2d int1 = Vector2d(start.X(), start.Y()) + (v0.Dot(v2) / v2.Dot(v2)) * v2;

         if (p1.X() != 0.0 && p1.Y() != 0.0)
            writer.OutputLine(int1, p1, delaunayStyle);
      }
   }
}
