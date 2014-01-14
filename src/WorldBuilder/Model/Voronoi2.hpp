//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file Voronoi2.hpp Voronoi diagram, using Boost.Polygon
//
#pragma once

// include
#include <boost/polygon/voronoi_diagram.hpp>
#include "Vector2d.hpp"

/// \brief classes for generating Voronoi diagram
/// \see http://en.wikipedia.org/wiki/Voronoi_diagram
namespace Voronoi2
{

/// Voronoi edge
struct Edge
{
   // voronoi edge points
   Vector2d v0;
   Vector2d v1;

   // delaunay line points
   Vector2d d0;
   Vector2d d1;
};

/// \brief Voronoi diagram
class Diagram
{
public:
   /// \brief ctor; constructs a voroni diagram
   Diagram(const std::vector<Vector2d>& vecPoints, const Vector2d& vBounds);

   // get methods

   /// returns number of sites or cells; may differ from input vecPoints
   size_t NumSites() const throw();

   /// returns all edges (no double edges)
   void GetAllEdges(std::vector<Edge>& vecAllEdges);

   /// returns region points (polygon points in ccw direction) for a given site index
   void Region(size_t index, std::vector<Vector2d>& vecRegionPoints);

   // actions

   /// outputs voronoi diagram as .svg file
   void OutputSvg(LPCTSTR pszFilename, bool bDrawArrows = false, bool bDrawDelaunay = false);

private:
   /// diagram type
   typedef boost::polygon::voronoi_diagram<double> diagram_type;

   /// voronoi diagram
   diagram_type m_diagram;

   /// bounds
   Vector2d m_vBounds;
};

} // namespace Voronoi2
