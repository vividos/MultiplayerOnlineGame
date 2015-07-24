//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ReduceAlgorithm.cpp Terrain triangles reduce algorithm
//

// includes
#include "StdAfx.h"
#include "ReduceAlgorithm.hpp"
#include "IActiveVertexMap.hpp"

using namespace Terrain::Reduce;

//
// ReduceAlgorithmBase
//

void ReduceAlgorithmBase::Reduce(IActiveVertexMap& activeVertexMap)
{
   for (unsigned int x=0; x<m_uiSize; x++)
   for (unsigned int y=0; y<m_uiSize; y++)
   {
      if (activeVertexMap.IsActive(x,y))
         continue;

      unsigned int level = std::min(GetBoundary(x), GetBoundary(y));
      if (x > level && y > level)
         DoQuad(activeVertexMap, x - level, y - level, level * 2);
   }
}

Vector3d ReduceAlgorithmBase::MapPoint(unsigned int x,unsigned  int y) const throw()
{
   ATLASSERT(x <= m_uiSize);
   ATLASSERT(y <= m_uiSize);

   return Vector3d(x, m_elevationData.Height(x,y), y);
}

/// \brief activates point in vertex map
/*! \details
This is tricky stuff.  When this is called, it means the given point is needed
for the terrain we are working on.  Each point, when activated, will recusivly 
require two other m_points at the next lowest level of detail.  This is what 
causes the "shattering" effect that breaks the terrain into triangles.  
If you want to know more, Google for Peter Lindstrom, the inventor of this 
very clever system.  
*/
void ReduceAlgorithmBase::ActivatePoint(IActiveVertexMap& activeVertexMap, int x, int y)
{
   if (x < 0 || y < 0 || !activeVertexMap.IsInMap(unsigned(x), unsigned(y)))
      return;

   if (activeVertexMap.IsActive(x,y))
      return;

   activeVertexMap.SetActive(x,y);

   if (unsigned(x) >= m_uiSize || 
       unsigned(y) >= m_uiSize)
       return; // TODO

   int xl = GetBoundary(x);
   int yl = GetBoundary(y);
   int level = std::min(xl, yl);

   if (xl > yl)
   {
      ActivatePoint(activeVertexMap, x - level, y);
      ActivatePoint(activeVertexMap, x + level, y);
   }
   else if (xl < yl)
   {
      ActivatePoint(activeVertexMap, x, y + level);
      ActivatePoint(activeVertexMap, x, y - level);
   }
   else
   {
      int x2 = x & (level * 2);
      int y2 = y & (level * 2);

      if (x2 == y2)
      {
         ActivatePoint(activeVertexMap, x - level, y + level);
         ActivatePoint(activeVertexMap, x + level, y - level);
      }
      else
      {
         ActivatePoint(activeVertexMap, x + level, y + level);
         ActivatePoint(activeVertexMap, x - level, y - level);
      }
   }
}

//
// ReduceAlgorithmCameraDistance
//

void ReduceAlgorithmCameraDistance::Reduce(IActiveVertexMap& activeVertexMap, const Vector3d& vCamera)
{
   m_vCamera = vCamera;

   ReduceAlgorithmBase::Reduce(activeVertexMap);
}

/// This controls the radius of the spherical area around the camera where
/// terrain data is concentrated.  That is, stuff right next to the camera is
/// the highest detail, and will diminish until it reaches FAR_VIEW, where detail
/// is lowest.
#define FAR_VIEW          200 // TODO rename

double ReduceAlgorithmCameraDistance::MapDistance(unsigned int /*x*/, unsigned int /*y*/)
{
/*
   Vector3d vDist = MapPoint(x,y) - m_vCamera;
   double dDist = vDist.Length() / FAR_VIEW;
   if (dDist < 0.0)
      dDist = 0.0;

   if (dDist > 1.0)
      dDist = 1.0;

   return dDist;
*/
   return 0.5;
}




/*! \details
\verbatim
            upper         
         ul-------ur        
          |\      |      
         l| \     |r     
         e|  \    |i      
         f|   c   |g    
         t|    \  |h         
          |     \ |t         
          |      \|          
         ll-------lr         
            lower            
\endverbatim
This considers a quad for splitting. This is done by looking to see how 
coplanar the quad is.  The elevation of the corners are averaged, and compared 
to the elevation of the center.  The greater the difference between these two 
values, the more non-coplanar this quad is.
*/
double ReduceAlgorithmCameraDistance::CalcDelta(unsigned int x1, unsigned int y1, unsigned int uiQuadSize) throw()
{
   unsigned int half = uiQuadSize / 2;
   unsigned int xc = x1 + half;
   unsigned int x2 = x1 + uiQuadSize;
   unsigned int yc = y1 + half;
   unsigned int y2 = y1 + uiQuadSize;

   unsigned int uiMapSize = GetSize();

   if (x2 > uiMapSize || y2 > uiMapSize)
      return 0.0;

   float ul, ur, ll, lr, center;
   Vector3d pos = MapPoint(x1, y1);  ul = (float)pos.Y();
   pos = MapPoint(x2, y1);  ur = (float)pos.Y();
   pos = MapPoint(x1, y2);  ll = (float)pos.Y();
   pos = MapPoint(x2, y2);  lr = (float)pos.Y();
   pos = MapPoint(xc, yc);  center = (float)pos.Y();

   float average = (ul + lr + ll + ur) / 4.0f;

   // look for a delta between the center point and the average elevation
   float delta = fabs((average - center)) * 5.0f;

   // scale the delta based on the size of the quad we are dealing with
   delta /= (float)uiQuadSize;

   // scale based on distance
   float dist = (float)MapDistance(xc, yc);
   delta *= (1.0f - (dist * 0.85f));

   // if the distance is very close, then we want a lot more detail
   if (dist < 0.15f)
      delta *= 10.0f;

   // smaller quads are much less important
   float size_bias = (float)(uiMapSize + uiQuadSize) / (float)(uiMapSize * 2);
   delta *= size_bias;

   return delta;
}

void ReduceAlgorithmCameraDistance::DoQuad(IActiveVertexMap& activeVertexMap, unsigned int x1, unsigned int y1, unsigned int size)
{
   double delta = CalcDelta(x1, y1, size);

   // MAGIC NUMBER: Fiddle with this to adjust how agressive the program should be 
   // in removing polygons.  Higher numbers result in fewer terrain polygons
   //const float c_tolerance = 0.07f / 100.0f;
   const float c_tolerance = 0.5f;

   if (delta > c_tolerance)
   //if (size > 8)
   {
      unsigned int half = size / 2;
      unsigned int xc = x1 + half;
      unsigned int yc = y1 + half;

      ActivatePoint(activeVertexMap, xc, yc);
   }
}




//
// ReduceAlgorithmLevelOfDetail
//

void ReduceAlgorithmLevelOfDetail::Reduce(IActiveVertexMap& activeVertexMap, unsigned int uiLevel)
{
   m_uiLevel = uiLevel;

   ReduceAlgorithmBase::Reduce(activeVertexMap);
}

void ReduceAlgorithmLevelOfDetail::DoQuad(IActiveVertexMap& activeVertexMap, unsigned int x1, unsigned int y1, unsigned int size)
{
   // MAGIC NUMBER: Fiddle with this to adjust how agressive the program should be 
   // in removing polygons.  Higher numbers result in fewer terrain polygons
   float tolerances[3] = { 0.05f, 0.5f, 0.8f };

   float tolerance = tolerances[m_uiLevel];

   double delta = CalcDelta(x1, y1, size);

   if (delta > tolerance)
   {
      unsigned int half = size / 2;
      unsigned int xc = x1 + half;
      unsigned int yc = y1 + half;

      ActivatePoint(activeVertexMap, xc, yc);
   }
}
