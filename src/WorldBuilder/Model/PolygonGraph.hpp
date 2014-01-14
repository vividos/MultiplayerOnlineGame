//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file PolygonGraph.hpp Polygon graph
//
#pragma once

// include
#include <vector>
#include <map>
#include "Vector2d.hpp"
#include "Color.hpp"

namespace PolygonGraph
{
// forward references
struct Center;
struct Corner;
struct Edge;

typedef std::shared_ptr<Center> CenterPtr;
typedef std::shared_ptr<Corner> CornerPtr;
typedef std::shared_ptr<Edge> EdgePtr;

/// terrain type
enum T_enTerrainType
{
   typeUnassigned = 0,
   typeLand,   ///< land
   typeOcean,  ///< ocean; unable to swim here
   typeLake,   ///< lake; inside of land polygons
   typeCoast,  ///< land polygon touching ocean
};

/// biome type
enum T_enBiomeType
{
   biomeUnassigned = 0,

   // biomes determined by type only
   biomeOcean,
   biomeLake,
   biomeIce,
   biomeMarsh,
   biomeBeach,

   // biomes for all other land polygons

   // elevation zone 4 (high)
   biomeSnow,
   biomeTundra,
   biomeBare,
   biomeScorched,

   // elevation zone 3
   biomeTaiga,
   biomeShrubland,
   biomeTemperateDesert,

   // elevation zone 2
   biomeTemperateRainForest,
   biomeTemperateDeciduousForest,
   biomeGrassland,

   // elevation zone 1 (low)
   biomeTropicalRainForest,
   biomeTropicalSeasonalForest,
   biomeSubtropicalDesert,   
};

Color ColorByBiomeType(T_enBiomeType enBiomeType) throw();
Color ColorByTerrainType(T_enTerrainType enTerrainType) throw();
Color ColorByElevation(double elevation) throw();
Color ColorByMoisture(double elevation) throw();

/// polygon center point
struct Center
{
   Center(size_t index_, const Vector2d& point_)
      :index(index_),
       point(point_),
       water(false),
       ocean(false),
       coast(false),
       border(false),
       enTerrainType(typeUnassigned),
       enBiomeType(biomeUnassigned),
       elevation(0.0),
       moisture(0.0)
   {
   }

   size_t index; ///< center index
   Vector2d point; ///< center point
   bool water;
   bool ocean;
   bool coast;
   bool border;
   T_enTerrainType enTerrainType;
   T_enBiomeType   enBiomeType;

   double elevation; // [0..1]
   double moisture; // [0..1]

   std::vector<CenterPtr> neighbors;
   std::vector<EdgePtr> borders;
   std::vector<CornerPtr> corners;
};

/// edge corner point
struct Corner
{
   Corner(size_t index_, const Vector2d& point_)
      :index(index_),
       point(point_),
       water(false),
       ocean(false),
       coast(false),
       border(false),
       enTerrainType(typeUnassigned),
       elevation(0.0),
       moisture(0.0),
       river(0)
   {
   }

   size_t index; ///< corner index
   Vector2d point; ///< corner point
   bool water;
   bool ocean;
   bool coast;
   bool border; ///< indicates if corner point is at border
   T_enTerrainType enTerrainType;

   double elevation; ///< [0..1]
   double moisture; ///< [0..1]

   unsigned int river; ///< 0 if no river, or volume of water in river

   CornerPtr downslope; ///< adjacent corner most downhill
   CornerPtr watershed; ///< coastal corner point, or null
   unsigned int watershedSize;

   std::vector<CenterPtr> touches;
   std::vector<EdgePtr> protrudes;
   std::vector<CornerPtr> adjacent;
};

/// polygon edge
struct Edge
{
   Edge(size_t index_)
      :index(index_),
       river(0)
   {
   }

   size_t index; ///< corner index

   CornerPtr v0; /// voronoi edge, point 0
   CornerPtr v1; /// voronoi edge, point 1

   CenterPtr d0; /// delaunay edge, point 0
   CenterPtr d1; /// delaunay edge, point 1

   Vector2d midpoint; ///< halfway between v0,v1

   unsigned int river;  ///< volume of water, or 0
};

/// sorts corner objects by angle to given center point
struct CornerSorter
{
   CornerSorter(const Vector2d& center)
      :m_center(center)
   {
   }

   bool operator()(const CornerPtr& lhs, const CornerPtr& rhs) const throw()
   {
      Vector2d lhv = m_center - lhs->point;
      Vector2d rhv = m_center - rhs->point;

      return lhv.PolarAngle() > rhv.PolarAngle();
   }

private:
   const Vector2d& m_center;
};

/// sorts polygon points by angle to given center point
struct PolygonPointSorter
{
   PolygonPointSorter(const Vector2d& center)
      :m_center(center)
   {
   }

   bool operator()(const Vector2d& lhs, const Vector2d& rhs) const throw()
   {
      Vector2d lhv = m_center - lhs;
      Vector2d rhv = m_center - rhs;

      return lhv.PolarAngle() < rhv.PolarAngle();
   }

   Vector2d m_center;
};



template <typename TPointObject>
class PointLookupMap
{
public:
   typedef TPointObject PointObjectType;
   typedef Vector2d PointType;
   typedef std::shared_ptr<PointObjectType> PointObjectPtrType;

   void Add(PointObjectPtrType sp)
   {
      m_lookupMap.insert(std::make_pair(sp->point, sp));
   }

   PointObjectPtrType Lookup(const PointType& p)
   {
      T_LookupMap::const_iterator iter = m_lookupMap.find(p);
      if (iter == m_lookupMap.end())
         return PointObjectPtrType();

      ATLASSERT(DoublesEqual((p - iter->first).Length(), 0.0)); // found point must be equal
      return iter->second;
   }

   void Clear()
   {
      m_lookupMap.clear();
   }

private:
   struct LessThan
   {
      bool operator()(const PointType& lhs, const PointType& rhs) const throw()
      {
         if (DoublesEqual((lhs-rhs).Length(), 0.0))
            return false; // equal

         if (DoublesEqual(lhs.X(), rhs.X())) // x being equal?
            return lhs.Y() < rhs.Y(); // order by y then
         else
            return lhs.X() < rhs.X();
      }
   };

   typedef std::map<PointType, PointObjectPtrType, LessThan> T_LookupMap;

   T_LookupMap m_lookupMap;
};


class Graph
{
public:
   void SetSize(unsigned int uiSizeX, unsigned int uiSizeY)
   {
      m_sizeX = uiSizeX;
      m_sizeY = uiSizeY;
   }

   /// clears the graph
   void Clear();

   /// returns number of centers
   size_t NumCenters() const throw() { return m_centers.size(); }

   /// returns number of corners
   size_t NumCorners() const throw() { return m_corners.size(); }

   /// return corner for point; create when not already there
   CornerPtr MakeCorner(const Vector2d& p);

   /// return list of land corners
   std::vector<CornerPtr> LandCorners() const;

   /// return edge that contains both corners
   EdgePtr LookupEdgeFromCorner(CornerPtr q, CornerPtr s) const;

   /// outputs current graph to .svg format
   void OutputSvg(LPCTSTR pszFilename) const;

public:
   double m_sizeX;
   double m_sizeY;

   std::vector<CenterPtr> m_centers;
   std::vector<CornerPtr> m_corners;
   std::vector<EdgePtr> m_edges;

   PointLookupMap<Center> m_centerLookup;
   PointLookupMap<Corner> m_cornerLookup;
};

} // namespace PolygonGraph
