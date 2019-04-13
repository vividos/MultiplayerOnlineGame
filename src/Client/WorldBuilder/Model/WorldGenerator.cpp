//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file WorldGenerator.cpp World generator
//

// include
#include "StdAfx.h"
#include "WorldGenerator.hpp"
#include "IslandShape.hpp"
#include "Voronoi2.hpp"
#include <functional>
#include <boost/foreach.hpp>

/// point density for generating random points; described as number of points per 64x64 square
//const double c_dPointDensity = 2.0 / 4096.0;
const double c_dPointDensity = 4.0 / 4096.0;

/// interation count for Lloyd Relaxation algorithm; see ImproveRandomPoints()
const unsigned int c_uiNumLloydIterations = 2;

/// lake threshold; range: 0 to 1, fraction of water corners for water polygon
const double c_dLakeThreshold = 0.3;

/// this factor increases the mountain area. At 1.0 the maximum elevation
/// barely shows up on the map, so we set it to 1.1.
const double c_dElevationScaleFactor = 1.1;

WorldGenerator::WorldGenerator(unsigned int uiSize)
:m_uiSize(uiSize)
{
   m_graph.SetSize(uiSize, uiSize);
}

void WorldGenerator::SetUpdateCallback(T_fnOnUpdate fnOnUpdate)
{
   m_fnOnUpdate = fnOnUpdate;
}

void WorldGenerator::UpdateStatus(const CString& cszStatus, bool bUpdatedGraph)
{
   if (m_fnOnUpdate)
      m_fnOnUpdate(cszStatus, bUpdatedGraph);
}

void WorldGenerator::Generate(T_enIslandShape enIslandShape, unsigned int uiSeed)
{
   ATLASSERT(m_uiSize > 0);

   m_graph.Clear();

   switch (enIslandShape)
   {
   case islandPerlinNoise:
      m_spIslandShape.reset(new PerlinIslandShape(uiSeed));
      break;
   case islandRadialRandom:
      m_spIslandShape.reset(new RadialIslandShape(uiSeed));
      break;
   case islandSquare:
      m_spIslandShape.reset(new SquareIslandShape);
      break;
   default:
      ATLASSERT(false);
      return;
   }

   // graph generation
   {
      UpdateStatus(_T("GenerateRandomPoints"));
      std::vector<Vector2d> vecPoints;
      GenerateRandomPoints(vecPoints);

      UpdateStatus(_T("ImproveRandomPoints"));
      ImproveRandomPoints(vecPoints);

      UpdateStatus(_T("BuildGraph"));
      BuildGraph(vecPoints);
      UpdateStatus(_T("ImproveCorners"), true);
      ImproveCorners();
   }

   // elevations
   {
      UpdateStatus(_T("AssignElevations"), true);
      AssignCornerElevations();
      AssignOceanCoastAndLand();
      RedistributeElevations();
      AssignWaterElevation();
      AssignPolygonElevations();
   }

   // river stuff
   {
      UpdateStatus(_T("CreateRivers"), true);
      CalculateDownslopes();
      CalculateWatersheds();
      CreateRivers();
   }

   // moisture

   // Determine moisture at corners, starting at rivers
   // and lakes, but not oceans. Then redistribute
   // moisture to cover the entire range evenly from 0.0
   // to 1.0. Then assign polygon moisture as the average
   // of the corner moisture.
   UpdateStatus(_T("AssignMoisture"), true);
   {
      AssignCornerMoisture();
      RedistributeMoisture();
      AssignPolygonMoisture();
   }

   // decorate map
   UpdateStatus(_T("AssignBiomes"), true);
   {
      AssignBiomes();
   }

   UpdateStatus(_T("Done."), true);
}

/// Generate random points and assign them to be on the island or
/// in the water. Some water points are inland lakes; others are
/// ocean. We'll determine ocean later by looking at what's
/// connected to ocean.
void WorldGenerator::GenerateRandomPoints(std::vector<Vector2d>& vecPoints)
{
   boost::uniform_int<> distX(10, m_uiSize-10);
   boost::uniform_int<> distY(10, m_uiSize-10);

   boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieX(m_rng, distX);
   boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieY(m_rng, distY);

   unsigned int uiNumPoints = unsigned((m_uiSize * m_uiSize) * c_dPointDensity);

   for (unsigned int i=0; i<uiNumPoints; i++)
      vecPoints.push_back(Vector2d(dieX(), dieY()));
}

/// We'd really like to generate "blue noise". Algorithms:
/// 1. Poisson dart throwing: check each new point against all
/// existing points, and reject it if it's too close.
/// 2. Start with a hexagonal grid and randomly perturb points.
/// 3. Lloyd Relaxation: move each point to the centroid of the
/// generated Voronoi polygon, then generate Voronoi again.
/// 4. Use force-based layout algorithms to push points away.
/// 5. More at http://www.cs.virginia.edu/~gfx/pubs/antimony/
/// Option 3 is implemented here. If it's run for too many iterations,
/// it will turn into a grid, but convergence is very slow, and we only
/// run it a few times.
void WorldGenerator::ImproveRandomPoints(std::vector<Vector2d>& vecPoints)
{
   for (unsigned int iIter=0; iIter < c_uiNumLloydIterations; iIter++)
   {
      Voronoi2::Diagram voronoi(vecPoints, Vector2d(m_uiSize, m_uiSize));

      vecPoints.resize(voronoi.NumSites());

      for (size_t iPoints=0; iPoints<voronoi.NumSites(); iPoints++)
      {
         // get region for point
         std::vector<Vector2d> vecRegion;
         voronoi.Region(iPoints, vecRegion);

         Vector2d vecP;
         for (size_t iRegionPt=0; iRegionPt<vecRegion.size(); iRegionPt++)
         {
            ATLASSERT(vecRegion[iRegionPt].X() >= 0 && vecRegion[iRegionPt].Y() >= 0);

            vecP += vecRegion[iRegionPt];
         }

         vecP *= 1.0 / vecRegion.size();

         vecPoints[iPoints] = Vector2d(int(vecP.X()), int(vecP.Y()));
      }
   }
}

/// adds a shared pointer to a vector if it doesn't already contain it
template <typename TElementPtr>
void AddElementToList(std::vector<TElementPtr>& vecElemList, TElementPtr spElem)
{
   using namespace PolygonGraph;

   ATLASSERT(spElem != NULL);

   // find in vecCorner
   auto iter = std::find_if(vecElemList.begin(), vecElemList.end(), [&spElem](TElementPtr spOther){
      return spElem == spOther;
   });

   if (iter == vecElemList.end())
   {
      // not found, so add it
      vecElemList.push_back(spElem);
   }
}

/// Build graph data structure in 'edges', 'centers', 'corners',
/// based on information in the Voronoi results: center.neighbors
/// will be a list of neighboring centers.
/// Each edge connects to four points: the Voronoi edge
/// edge.{v0,v1} and its dual Delaunay triangle edge edge.{d0,d1}.
void WorldGenerator::BuildGraph(std::vector<Vector2d>& vecPoints)
{
   Voronoi2::Diagram voronoi(vecPoints, Vector2d(m_uiSize, m_uiSize));

   using namespace PolygonGraph;

   // Build Center objects for each of the points, and a lookup map
   // to find those Center objects again as we build the graph
   {
      m_graph.m_centers.reserve(vecPoints.size());

      for (size_t i=0, iMax=vecPoints.size(); i<iMax; i++)
      {
         const Vector2d& point = vecPoints[i];

         size_t nextIndex = m_graph.m_centers.size();

         CenterPtr spCenter = CenterPtr(new Center(nextIndex, point));

         // there must be no double points
         ATLASSERT(m_graph.m_centerLookup.Lookup(point) == NULL);

         m_graph.m_centers.push_back(spCenter);

         m_graph.m_centerLookup.Add(spCenter);
      }
   }

   // The Voronoi library generates multiple Point objects for
   // corners, and we need to canonicalize to one Corner object.
   // To make lookup fast, we keep an array of Points, bucketed by
   // x value, and then we only have to look at other Points in
   // nearby buckets. When we fail to find one, we'll create a new
   // Corner object.
   {
      // get list of all segments
      std::vector<Voronoi2::Edge> vecAllEdges;

      voronoi.GetAllEdges(vecAllEdges);

      for (const Voronoi2::Edge& e : vecAllEdges)
      {
         size_t nextIndex = m_graph.m_edges.size();

         PolygonGraph::EdgePtr spEdge =
            PolygonGraph::EdgePtr(new PolygonGraph::Edge(nextIndex));

         spEdge->v0 = m_graph.MakeCorner(e.v0);
         spEdge->v1 = m_graph.MakeCorner(e.v1);

         spEdge->d0 = m_graph.m_centerLookup.Lookup(e.d0);
         spEdge->d1 = m_graph.m_centerLookup.Lookup(e.d1);

         // center points must already exist
         ATLASSERT(spEdge->d0 != NULL);
         ATLASSERT(spEdge->d1 != NULL);

         // calc midpoint
         spEdge->midpoint = Interpolate<Vector2d>(spEdge->v0->point, spEdge->v1->point, 0.5);

         // Centers point to edges. Corners point to edges.
         spEdge->d0->borders.push_back(spEdge);
         spEdge->d1->borders.push_back(spEdge);
         spEdge->v0->protrudes.push_back(spEdge);
         spEdge->v1->protrudes.push_back(spEdge);

         // Centers point to centers.
         AddElementToList(spEdge->d0->neighbors, spEdge->d1);
         AddElementToList(spEdge->d1->neighbors, spEdge->d0);

         // Corners point to corners
         AddElementToList(spEdge->v0->adjacent, spEdge->v1);
         AddElementToList(spEdge->v1->adjacent, spEdge->v0);

         // Centers point to corners
         AddElementToList(spEdge->d0->corners, spEdge->v0);
         AddElementToList(spEdge->d0->corners, spEdge->v1);

         AddElementToList(spEdge->d1->corners, spEdge->v0);
         AddElementToList(spEdge->d1->corners, spEdge->v1);

         // Corners point to centers
         AddElementToList(spEdge->v0->touches, spEdge->d0);
         AddElementToList(spEdge->v0->touches, spEdge->d1);

         AddElementToList(spEdge->v1->touches, spEdge->d0);
         AddElementToList(spEdge->v1->touches, spEdge->d1);

         m_graph.m_edges.push_back(spEdge);
      }
   }
}

/// Although Lloyd relaxation improves the uniformity of polygon
/// sizes, it doesn't help with the edge lengths. Short edges can
/// be bad for some games, and lead to weird artifacts on
/// rivers. We can easily lengthen short edges by moving the
/// corners, but **we lose the Voronoi property**. The corners are
/// moved to the average of the polygon centers around them. Short
/// edges become longer. Long edges tend to become shorter. The
/// polygons tend to be more uniform after this step.
void WorldGenerator::ImproveCorners()
{
   std::vector<Vector2d> vecNewCorners(m_graph.m_corners.size());

   // First we compute the average of the centers next to each corner.
   for (size_t i=0, iMax=m_graph.m_corners.size(); i<iMax; i++)
   {
      PolygonGraph::CornerPtr c = m_graph.m_corners[i];

      if (c->border)
      {
         // keep point
         vecNewCorners[i] = c->point;
      }
      else
      {
         Vector2d point;

         for (PolygonGraph::CenterPtr spCenter : c->touches)
         {
            point += spCenter->point;
         }

         point *= 1.0 / c->touches.size();

         vecNewCorners[i] = point;
      }
   }

   // Move the corners to the new locations.
   for (size_t i=0, iMax=m_graph.m_corners.size(); i<iMax; i++)
      m_graph.m_corners[i]->point = vecNewCorners[i];

   // The edge midpoints were computed for the old corners and need
   // to be recomputed.
   for (PolygonGraph::EdgePtr spEdge : m_graph.m_edges)
   {
      ATLASSERT(spEdge->v0 != NULL);
      ATLASSERT(spEdge->v1 != NULL);

      spEdge->midpoint = Interpolate(spEdge->v0->point, spEdge->v1->point, 0.5);
   }
}

bool WorldGenerator::IsInside(const Vector2d& point) const
{
   if (m_spIslandShape == NULL)
      return false;

   // convert point to range [-0.5; 0.5], for x and y each
   double dx = 2.0*(point.X()/m_uiSize - 0.5);
   double dy = 2.0*(point.Y()/m_uiSize - 0.5);

   return m_spIslandShape->IsInIsland(Vector2d(dx, dy));
}

/// Determine elevations and water at Voronoi corners. By
/// construction, we have no local minima. This is important for
/// the downslope vectors later, which are used in the river
/// construction algorithm. Also by construction, inlets/bays
/// push low elevation areas inland, which means many rivers end
/// up flowing out through them. Also by construction, lakes
/// often end up on river paths because they don't raise the
/// elevation as much as other terrain does.
void WorldGenerator::AssignCornerElevations()
{
   std::deque<PolygonGraph::CornerPtr> deqCorners;

   for (PolygonGraph::CornerPtr c : m_graph.m_corners)
   {
      c->water = !IsInside(c->point);

      // The edges of the map are elevation 0
      if (c->border)
      {
         c->elevation = 0.0;
         deqCorners.push_back(c);
      }
      else
      {
         c->elevation = std::numeric_limits<double>::infinity();
      }
   }

   // Traverse the graph and assign elevations to each point. As we
   // move away from the map border, increase the elevations. This
   // guarantees that rivers always have a way down to the coast by
   // going downhill (no local minima).
   while (!deqCorners.empty())
   {
      PolygonGraph::CornerPtr c = deqCorners.front();
      deqCorners.pop_front();

      for (PolygonGraph::CornerPtr adj : c->adjacent)
      {
         // Every step up is epsilon over water or 1 over land. The
         // number doesn't matter because we'll rescale the
         // elevations later.
         double newElevation = c->elevation + 0.01;
         if (!c->water && !adj->water)
            newElevation += 1.0;

         // If this point changed, we'll add it to the queue so
         // that we can process its neighbors too.
         if (newElevation < adj->elevation)
         {
            adj->elevation = newElevation;
            deqCorners.push_back(adj);
         }
      }
   }
}

/// Compute polygon attributes 'ocean' and 'water' based on the
/// corner attributes. Count the water corners per
/// polygon. Oceans are all polygons connected to the edge of the
/// map. In the first pass, mark the edges of the map as ocean;
/// in the second pass, mark any water-containing polygon
/// connected an ocean as ocean.
void WorldGenerator::AssignOceanCoastAndLand()
{
   std::deque<PolygonGraph::CenterPtr> deqCenters;

   for (PolygonGraph::CenterPtr p : m_graph.m_centers)
   {
      unsigned int numWater = 0;

      for (PolygonGraph::CornerPtr q : p->corners)
      {
         if (q->border)
         {
            p->border = true;
            p->ocean = true;

            q->water = true;

            deqCenters.push_back(p);
         }

         if (q->water)
            numWater++;
      }

      p->water = p->ocean ||
         numWater >= unsigned(p->corners.size() * c_dLakeThreshold);
   }

   while (!deqCenters.empty())
   {
      PolygonGraph::CenterPtr p = deqCenters.front();
      deqCenters.pop_front();

      for (PolygonGraph::CenterPtr neighborCenter : p->neighbors)
      {
         if (neighborCenter->water && !neighborCenter->ocean)
         {
            neighborCenter->ocean = true;
            deqCenters.push_back(neighborCenter);
         }
      }
   }

   // Set the polygon attribute 'coast' based on its neighbors. If
   // it has at least one ocean and at least one land neighbor,
   // then this is a coastal polygon.
   for (PolygonGraph::CenterPtr p : m_graph.m_centers)
   {
      unsigned int numOcean = 0;
      unsigned int numLand = 0;

      for (PolygonGraph::CenterPtr neighborCenter : p->neighbors)
      {
         if (neighborCenter->ocean)
            numOcean++;

         if (!neighborCenter->water)
            numLand++;
      }

      if (numOcean > 0 && numLand > 0)
         p->coast = true;
   }

   // Set the corner attributes based on the computed polygon
   // attributes. If all polygons connected to this corner are
   // ocean, then it's ocean; if all are land, then it's land;
   // otherwise it's coast.
   for (PolygonGraph::CornerPtr q : m_graph.m_corners)
   {
      unsigned int numOcean = 0;
      unsigned int numLand = 0;

      for (size_t i=0, iMax=q->touches.size(); i<iMax; i++)
      {
         PolygonGraph::CenterPtr p = q->touches[i];

         if (p->ocean)
            numOcean++;

         if (!p->water)
            numLand++;
      }

      if (numOcean == q->touches.size())
         q->ocean = true;
      else
      if (numOcean > 0 && numLand > 0)
         q->coast = true;

      q->water = q->border ||
         ((numLand != q->touches.size()) && !q->coast);
   }



   // assign terrain type, at last
   for (PolygonGraph::CenterPtr p : m_graph.m_centers)
   {
      if (p->border || p->ocean)
      {
         ATLASSERT(p->water == true);
         p->enTerrainType = PolygonGraph::typeOcean;
      }
      else
      if (!p->ocean && p->water)
         p->enTerrainType = PolygonGraph::typeLake;
      else
      if (p->coast)
         p->enTerrainType = PolygonGraph::typeCoast;
      else
         p->enTerrainType = PolygonGraph::typeLand;
   }
}

template <typename TElementPtr>
struct ElevationSorter
{
   bool operator()(const TElementPtr& lhs, const TElementPtr& rhs) const
   {
      return lhs->elevation < rhs->elevation;
   }
};


/// Rescale elevations so that the highest is 1.0, and they're
/// distributed well. We want lower elevations to be more common
/// than higher elevations, in proportions approximately matching
/// concentric rings. That is, the lowest elevation is the
/// largest ring around the island, and therefore should more
/// land area than the highest elevation, which is the very
/// center of a perfectly circular island.
void WorldGenerator::RedistributeElevations()
{
   std::vector<PolygonGraph::CornerPtr> vecLocations = m_graph.LandCorners();

   // Change the overall distribution of elevations so that lower
   // elevations are more common than higher
   // elevations. Specifically, we want elevation X to have frequency
   // (1-X). To do this we will sort the corners, then set each
   // corner to its desired elevation.

   // sort on elevation
   std::sort(vecLocations.begin(), vecLocations.end(),
      ElevationSorter<PolygonGraph::CornerPtr>());

   for (size_t i=0; i<vecLocations.size(); i++)
   {
      // Let y(x) be the total area that we want at elevation <= x.
      // We want the higher elevations to occur less than lower
      // ones, and set the area to be y(x) = 1 - (1-x)^2.
      double y = i / double(vecLocations.size() - 1);

      // Now we have to solve for x, given the known y.
      // * y = 1 - (1-x)^2
      // * y = 1 - (1 - 2x + x^2)
      // * y = 2x - x^2
      // * x^2 - 2x + y = 0
      // From this we can use the quadratic equation to get:
      double x = sqrt(c_dElevationScaleFactor) - sqrt(c_dElevationScaleFactor*(1.0-y));
      if (x > 1.0)
         x = 1.0; // TODO: does this break downslopes?

      vecLocations[i]->elevation = x;
   }
}

void WorldGenerator::AssignWaterElevation()
{
   for (PolygonGraph::CornerPtr q : m_graph.m_corners)
   {
      if (q->ocean || q->coast)
         q->elevation = 0.0;
   }
}

void WorldGenerator::AssignPolygonElevations()
{
   for (PolygonGraph::CenterPtr p : m_graph.m_centers)
   {
      double sumElevation = 0.0;

      for (PolygonGraph::CornerPtr q : p->corners)
         sumElevation += q->elevation;

      p->elevation = sumElevation / p->corners.size();
   }
}

/// Calculate downslope pointers. At every point, we point to the
/// point downstream from it, or to itself. This is used for
/// generating rivers and watersheds.
void WorldGenerator::CalculateDownslopes()
{
   for (PolygonGraph::CornerPtr q : m_graph.m_corners)
   {
      PolygonGraph::CornerPtr r = q;

      for (PolygonGraph::CornerPtr s : q->adjacent)
      {
         if (s->elevation <= r->elevation)
            r = s;
      }

      q->downslope = r;
   }
}

/// Calculate the watershed of every land point. The watershed is
/// the last downstream land point in the downslope graph. TODO:
/// watersheds are currently calculated on corners, but it'd be
/// more useful to compute them on polygon centers so that every
/// polygon can be marked as being in one watershed.
void WorldGenerator::CalculateWatersheds()
{
   // Initially the watershed pointer points downslope one step.
   for (PolygonGraph::CornerPtr q : m_graph.m_corners)
   {
      q->watershed = q;
      if (!q->ocean && !q->coast)
         q->watershed = q->downslope;
   }

   // Follow the downslope pointers to the coast. Limit to 100
   // iterations although most of the time with NUM_POINTS=2000 it
   // only takes 20 iterations because most points are not far from
   // a coast. TODO: can run faster by looking at
   // p.watershed.watershed instead of p.downslope.watershed.
   for (unsigned int i=0; i<100; i++)
   {
      bool changed = false;
      for (PolygonGraph::CornerPtr q : m_graph.m_corners)
      {
         if (!q->ocean && !q->coast && !q->watershed->coast)
         {
            PolygonGraph::CornerPtr r = q->downslope->watershed;
            if (!r->ocean)
               q->watershed = r;
            changed = true;
         }
      }
      if (!changed)
      {
         ATLTRACE(_T("CalculateWatersheds(): exiting at iteration %i\n"), i);
         break;
      }
   }

   // How big is each watershed?
   for (PolygonGraph::CornerPtr q : m_graph.m_corners)
   {
      PolygonGraph::CornerPtr r = q->watershed;
      r->watershedSize = 1 + r->watershedSize; // TODO code ok?
   }
}

/// Create rivers along edges. Pick a random corner point, then
/// move downslope. Mark the edges and corners as rivers.
void WorldGenerator::CreateRivers()
{
   boost::uniform_int<> dist(0, m_graph.m_corners.size()-1);
   boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(m_rng, dist);

   const unsigned int c_uiRiverCount = 40; //m_uiSize / 2
   for (unsigned int i=0; i<c_uiRiverCount; i++)
   {
      size_t nextIndex = die();

      PolygonGraph::CornerPtr q = m_graph.m_corners[nextIndex];

      // Bias rivers to go west: if (q->downslope->point.X() > q->point.X()) continue;

      while (!q->coast)
      {
         if (q == q->downslope)
            break; // infinite loop

         if (q == q->watershed)
            break; // infinite loop

         PolygonGraph::EdgePtr edge = m_graph.LookupEdgeFromCorner(q, q->downslope);
         ATLASSERT(edge != NULL); // edge must be found

         edge->river++;
         q->river++; // TODO code ok? was: q.river = (q.river || 0) + 1;
         q->downslope->river++; // code ok? was: q.downslope.river = (q.downslope.river || 0) + 1;  // TODO: fix double count

         q = q->downslope;
      }
   }
}

/// Calculate moisture. Freshwater sources spread moisture: rivers
/// and lakes (not oceans). Saltwater sources have moisture but do
/// not spread it (we set it at the end, after propagation).
void WorldGenerator::AssignCornerMoisture()
{
   std::deque<PolygonGraph::CornerPtr> deqCorners;

   // Fresh water
   for (PolygonGraph::CornerPtr q : m_graph.m_corners)
   {
      if ((q->water || q->river > 0) && !q->ocean)
      {
         q->moisture = q->river > 0 ? std::min(3.0, (0.2 * q->river)) : 1.0;
         deqCorners.push_back(q);
      }
      else
      {
         q->moisture = 0.0;
      }
   }

   while (!deqCorners.empty())
   {
      PolygonGraph::CornerPtr q = deqCorners.front();
      deqCorners.pop_front();

      for (PolygonGraph::CornerPtr r : q->adjacent)
      {
         double newMoisture = q->moisture * 0.9;
         if (newMoisture > r->moisture)
         {
            r->moisture = newMoisture;
            deqCorners.push_back(r);
         }
      }
   }

   // Salt water
   for (PolygonGraph::CornerPtr q : m_graph.m_corners)
   {
      if (q->ocean || q->coast)
         q->moisture = 1.0;
   }
}

template <typename TElementPtr>
struct MoistureSorter
{
   bool operator()(const TElementPtr& lhs, const TElementPtr& rhs) const
   {
      return lhs->moisture < rhs->moisture;
   }
};

void WorldGenerator::RedistributeMoisture()
{
   std::vector<PolygonGraph::CornerPtr> vecLocations = m_graph.LandCorners();

   // sort on moisture
   std::sort(vecLocations.begin(), vecLocations.end(),
      MoistureSorter<PolygonGraph::CornerPtr>());

   for (size_t i=0, iMax=vecLocations.size(); i<iMax; i++)
   {
      vecLocations[i]->moisture = i / (vecLocations.size() - 1);
   }
}

void WorldGenerator::AssignPolygonMoisture()
{
   for (PolygonGraph::CenterPtr p : m_graph.m_centers)
   {
      double sumMoisture = 0.0;

      for (PolygonGraph::CornerPtr q : p->corners)
      {
         if (q->moisture > 1.0)
            q->moisture = 1.0;
         sumMoisture += q->moisture;
      }

      p->moisture = sumMoisture / p->corners.size();
   }
}

/// Assign a biome type for given polygon. If it has
/// ocean/coast/water, then that's the biome; otherwise it depends
/// on low/high elevation and low/medium/high moisture. This is
/// roughly based on the Whittaker diagram but adapted to fit the
/// needs of the island map generator.
/// \see http://www.marietta.edu/~biol/biomes/biome_main.htm
PolygonGraph::T_enBiomeType GetBiomeType(PolygonGraph::CenterPtr p)
{
   using namespace PolygonGraph;

   if (p->ocean)
      return biomeOcean;

   if (p->water)
   {
      if (p->elevation < 0.1) return biomeMarsh;
      if (p->elevation > 0.8) return biomeIce;
      return biomeLake;
   }

   if (p->coast)
      return biomeBeach;

   // elevation zone 4 (high)
   if (p->elevation > 0.8)
   {
      if (p->moisture > 0.50) return biomeSnow;
      if (p->moisture > 0.33) return biomeTundra;
      if (p->moisture > 0.16) return biomeBare;
      return biomeScorched;
   }

   // elevation zone 3
   if (p->elevation > 0.6)
   {
      if (p->moisture > 0.66) return biomeTaiga;
      if (p->moisture > 0.33) return biomeShrubland;
      return biomeTemperateDesert;
   }

   // elevation zone 2
   if (p->elevation > 0.3)
   {
      if (p->moisture > 0.83) return biomeTemperateRainForest;
      if (p->moisture > 0.50) return biomeTemperateDeciduousForest;
      if (p->moisture > 0.16) return biomeGrassland;
      return biomeTemperateDesert;
   }

   // elevation zone 1 (low)
   {
      if (p->moisture > 0.66) return biomeTropicalRainForest;
      if (p->moisture > 0.33) return biomeTropicalSeasonalForest;
      if (p->moisture > 0.16) return biomeGrassland;
      return biomeSubtropicalDesert;
   }
}

void WorldGenerator::AssignBiomes()
{
   for (PolygonGraph::CenterPtr p : m_graph.m_centers)
      p->enBiomeType = GetBiomeType(p);
}
