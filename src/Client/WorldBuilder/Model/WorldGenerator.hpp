//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file WorldGenerator.hpp World generator
//
#pragma once

// include
#include <vector>
#include "Vector2.hpp"
#include "PolygonGraph.hpp"
#include <boost/random.hpp>
#include <functional>

class IslandShape;

/// \brief world generator
/// \details generates square worlds using PolygonGraph::Graph
/// \see http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/
/// \see https://github.com/amitp/mapgen2/blob/master/Map.as
class WorldGenerator
{
public:
   /// generated island shape
   enum T_enIslandShape
   {
      islandPerlinNoise,   ///< generate island using perlin noise
      islandRadialRandom,  ///< generate island using radial distribution
      islandSquare,        ///< generate square island
   };

   /// ctor
   WorldGenerator(unsigned int uiSize) throw();

   /// update function
   typedef std::function<void(const CString& cszStatus, bool bUpdatedGraph)> T_fnOnUpdate;

   /// sets update callback function
   void SetUpdateCallback(T_fnOnUpdate fnOnUpdate);

   // actions

   /// generates the world; could take some time...
   void Generate(T_enIslandShape enIslandShape, unsigned int uiSeed);

   /// returns polygon graph
   const PolygonGraph::Graph& GetGraph() const throw() { return m_graph; }

private:
   // graph generation

   /// Generate random points for polygons
   void GenerateRandomPoints(std::vector<Vector2d>& vecPoints);

   /// Improve the random set of points with Lloyd Relaxation
   void ImproveRandomPoints(std::vector<Vector2d>& vecPoints);

   /// Build graph from random points
   void BuildGraph(std::vector<Vector2d>& vecPoints);

   /// Improve corners by adjusting length of edges
   void ImproveCorners();

   // elevation

   /// Determine the elevations and water at Voronoi corners
   void AssignCornerElevations();

   /// Determine polygon and corner type: ocean, coast, land
   void AssignOceanCoastAndLand();

   /// Rescale elevations so that they're distributed well
   void RedistributeElevations();

   /// Assign elevations to non-land corners
   void AssignWaterElevation();

   /// Polygon elevations are the average of the elevations of their corners
   void AssignPolygonElevations();

   // river stuff

   /// Determine downslope paths
   void CalculateDownslopes();

   /// Determine watersheds: for every corner, where does it flow out into the ocean?
   void CalculateWatersheds();

   /// Create rivers
   void CreateRivers();

   // moisture

   /// Determine moisture at corners
   void AssignCornerMoisture();

   /// Change the overall distribution of moisture to be evenly distributed
   void RedistributeMoisture();

   /// Polygon moisture is the average of the moisture at corners
   void AssignPolygonMoisture();

   // map decoration

   /// Assign a biome type to each polygon
   void AssignBiomes();

   // helper functions

   /// updates status text
   void UpdateStatus(const CString& cszStatus, bool bUpdatedGraph = false);

   /// Determine whether a given point should be on the island or in the water
   bool IsInside(const Vector2d& point) const;

private:
   /// update callback
   T_fnOnUpdate m_fnOnUpdate;

   /// map x and y size
   unsigned int m_uiSize;

   /// RNG
   boost::mt19937 m_rng;

   /// polygon graph
   PolygonGraph::Graph m_graph;

   /// island shape
   std::shared_ptr<IslandShape> m_spIslandShape;

   /// elevation data for the whole world
   std::vector<double> m_vecElevationData;

   /// humidity data for each page; 0 means dry, 255 means humid
   std::vector<unsigned char> m_vecHumidityData;

   /// temperature data for each page; 0 cold, 255 means hot
   std::vector<unsigned char> m_vecTemperatureData;
};
