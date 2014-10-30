//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file PolygonGraph.cpp Polygon graph
//

// include
#include "StdAfx.h"
#include "PolygonGraph.hpp"
#include "SVGOutput.hpp"
#include <boost/foreach.hpp>

using namespace PolygonGraph;

//
// global functions
//

Color PolygonGraph::ColorByBiomeType(T_enBiomeType enBiomeType) throw()
{
   switch (enBiomeType)
   {
   case biomeUnassigned: return Color::Red();

   // case biomes determined by type only
   case biomeOcean: return Color(0x44,0x44,0x7a);
   case biomeLake: return Color(0x33,66,99);
   case biomeIce: return Color(0x99,0xff,0xff);
   case biomeMarsh: return Color(0x2f,0x66,0x66);
   case biomeBeach: return Color(0xa0,0x90,0x77);

   // biomes for all other land polygons

   // elevation zone 4 (high)
   case biomeSnow: return Color(0xff,0xff,0xff);
   case biomeTundra: return Color(0xbb,0xbb,0xaa);
   case biomeBare: return Color(0x88,0x88,0x88);
   case biomeScorched: return Color(0x55,0x55,0x55);

   // elevation zone 3
   case biomeTaiga: return Color(0x99,0xaa,0x77);
   case biomeShrubland: return Color(0x88,0x99,0x77);
   case biomeTemperateDesert: return Color(0xc9,0xd2,0x9b);

   // elevation zone 2
   case biomeTemperateRainForest: return Color(0x44,0x88,0x55);
   case biomeTemperateDeciduousForest: return Color(0x67,0x94,0x59);
   case biomeGrassland: return Color(0x88,0xaa,0x55);

   // elevation zone 1 (low)
   case biomeTropicalRainForest: return Color(0x33,0x77,0x55);
   case biomeTropicalSeasonalForest: return Color(0x55,0x99,0x44);
   case biomeSubtropicalDesert: return Color(0xd2,0xb9,0x8b);

   default:
      ATLASSERT(false);
   }

   return Color::Red();
}

Color PolygonGraph::ColorByTerrainType(T_enTerrainType enTerrainType) throw()
{         
   return enTerrainType == PolygonGraph::typeOcean ? Color(52,58,94) :
      enTerrainType == PolygonGraph::typeLake ? Color(95,134,169) :
      enTerrainType == PolygonGraph::typeCoast ? Color(205,203,120) :
      Color(178,166,148);
}

Color PolygonGraph::ColorByElevation(double elevation) throw()
{
   ATLASSERT(elevation >= 0.0 && elevation <= 1.0);
   return Interpolate(Color(14,142,14), Color(252,253,16), elevation);
}

Color PolygonGraph::ColorByMoisture(double moisture) throw()
{
   ATLASSERT(moisture >= 0.0 && moisture <= 1.0);
   return Interpolate(Color(177,164,72), Color(81,113,254), moisture);
}


//
// Graph
//

void Graph::Clear()
{
   m_centerLookup.Clear();
   m_cornerLookup.Clear();

   // as all center, corner and edge objects hold each other, first clear all objects before removing them
   std::for_each(m_centers.begin(), m_centers.end(), std::bind(&Center::Clear, std::placeholders::_1));
   std::for_each(m_corners.begin(), m_corners.end(), std::bind(&Corner::Clear, std::placeholders::_1));
   std::for_each(m_edges.begin(), m_edges.end(), std::bind(&Edge::Clear, std::placeholders::_1));

   m_centers.clear();
   m_corners.clear();
   m_edges.clear();
}

CornerPtr Graph::MakeCorner(const Vector2d& p)
{
   CornerPtr sp = m_cornerLookup.Lookup(p);

   if (sp == CornerPtr())
   {
      size_t nextIndex = m_corners.size();

      sp = CornerPtr(new Corner(nextIndex, p));

      sp->border =
         DoublesEqual(p.X(), 0.0) ||
         DoublesEqual(p.Y(), 0.0) ||
         DoublesEqual(p.X(), m_sizeX) ||
         DoublesEqual(p.Y(), m_sizeX);

      m_corners.push_back(sp);

      m_cornerLookup.Add(sp);
   }

   return sp;
}

/// Create an array of corners that are on land only, for use by
/// algorithms that work only on land.
std::vector<CornerPtr> Graph::LandCorners() const
{
   std::vector<CornerPtr> vecLocations;

   BOOST_FOREACH(const CornerPtr q, m_corners)
   {
      if (!q->ocean && !q->coast)
         vecLocations.push_back(q);
   }

   return vecLocations;
}


EdgePtr Graph::LookupEdgeFromCorner(CornerPtr q, CornerPtr s) const
{
   BOOST_FOREACH(EdgePtr edge, q->protrudes)
   {
      if (edge->v0 == s || edge->v1 == s)
         return edge;
   }

   return EdgePtr();
}

void PolygonGraph::Graph::OutputSvg(LPCTSTR pszFilename) const
{
   SVG::Writer w(pszFilename, unsigned(m_sizeX), unsigned(m_sizeY));

   {
      SVG::Style borderPolygonStyle;
      borderPolygonStyle.Add(_T("fill"), _T("rgb(255,255,255)"));
      borderPolygonStyle.Add(_T("stroke"), _T("rgb(0,0,0)"));
      borderPolygonStyle.Add(_T("stroke-width"), 1);

      SVG::Style oceanPolygonStyle;
      oceanPolygonStyle.Add(_T("fill"), _T("rgb(52,58,94)"));
      oceanPolygonStyle.Add(_T("stroke"), _T("rgb(0,0,0)"));
      oceanPolygonStyle.Add(_T("stroke-width"), 1);

      SVG::Style landPolygonStyle;
      landPolygonStyle.Add(_T("fill"), _T("rgb(178,166,148)"));
      landPolygonStyle.Add(_T("stroke"), _T("rgb(0,0,0)"));
      landPolygonStyle.Add(_T("stroke-width"), 1);

      SVG::Style lakePolygonStyle;
      lakePolygonStyle.Add(_T("fill"), _T("rgb(95,134,169)"));
      lakePolygonStyle.Add(_T("stroke"), _T("rgb(0,0,0)"));
      lakePolygonStyle.Add(_T("stroke-width"), 1);

      SVG::Style coastPolygonStyle;
      coastPolygonStyle.Add(_T("fill"), _T("rgb(205,203,120)"));
      coastPolygonStyle.Add(_T("stroke"), _T("rgb(0,0,0)"));
      coastPolygonStyle.Add(_T("stroke-width"), 1);

      // output all centers
      for (size_t i=0, iMax=m_centers.size(); i<iMax; i++)
      {
         const Center& c = *m_centers[i];

         std::vector<Vector2d> vecPolygonPoints;
         for (size_t i=0, iMax=c.corners.size(); i<iMax; i++)
            vecPolygonPoints.push_back(c.corners[i]->point);

         // order them clockwise
         std::sort(vecPolygonPoints.begin(), vecPolygonPoints.end(), PolygonPointSorter(c.point));

         w.OutputPolygon(vecPolygonPoints,
            //c.border ? borderPolygonStyle :
            c.enTerrainType == typeOcean ? oceanPolygonStyle :
            c.enTerrainType == typeLake ? lakePolygonStyle :
            c.enTerrainType == typeCoast ? coastPolygonStyle :
            landPolygonStyle
         );
      }
   }

   // output all rivers
   {
      SVG::Style riverStyle;
      riverStyle.Add(_T("stroke"), _T("rgb(113,116,242)"));
      riverStyle.Add(_T("stroke-width"), 5);

      BOOST_FOREACH(PolygonGraph::EdgePtr spEdge, m_edges)
      {
         if (spEdge->v0->river > 0 && spEdge->v1->river > 0)
            w.OutputLine(spEdge->v0->point, spEdge->v1->point, riverStyle);
      }
   }

   {
      SVG::Style centerStyle;
      centerStyle.Add(_T("stroke"), _T("rgb(0,0,0)"));
      centerStyle.Add(_T("stroke-width"), 2);

      for (size_t i=0, iMax=m_centers.size(); i<iMax; i++)
      {
         const Center& c = *m_centers[i];
         w.OutputCircle(c.point, 3.0, centerStyle);
      }
   }

/*
   {
      SVG::Style voronoiLineStyle;
      voronoiLineStyle.Add(_T("stroke"), _T("rgb(255,255,255)"));
      voronoiLineStyle.Add(_T("stroke-width"), 2);

      SVG::Style delaunayLineStyle;
      delaunayLineStyle.Add(_T("stroke"), _T("rgb(0,0,0)"));
      delaunayLineStyle.Add(_T("stroke-width"), 1);

      for (size_t i=0, iMax=m_edges.size(); i<iMax; i++)
      {
         const Edge& e = *m_edges[i];

         // delaunay line
         if (bDrawDelaunay)
            w.OutputLine(e.d0->point, e.d1->point, delaunayLineStyle);

         // voronoi edge
         w.OutputLine(e.v0->point, e.v1->point, voronoiLineStyle);
      }
   }

   {
      SVG::Style centerStyle;
      centerStyle.Add(_T("stroke"), _T("rgb(255,0,0)"));
      centerStyle.Add(_T("stroke-width"), 2);

      for (size_t i=0, iMax=m_centers.size(); i<iMax; i++)
      {
         const Center& c = *m_centers[i];
         w.OutputCircle(c.point, 7.0, centerStyle);
      }
   }

   {
      SVG::Style cornerStyle;
      cornerStyle.Add(_T("stroke"), _T("rgb(0,0,255)"));
      cornerStyle.Add(_T("stroke-width"), 2);

      for (size_t i=0, iMax=m_corners.size(); i<iMax; i++)
      {
         const Corner& c = *m_corners[i];
         w.OutputCircle(c.point, 5.0, cornerStyle);
      }
   }
*/
}
