//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file Voronoi.hpp Voronoi diagram
//
#pragma once

// include
#include <queue>
#include <map>
#include <cmath>
#include "Vector2.hpp"

/// \brief classes for generating Voronoi diagram
/// \see http://en.wikipedia.org/wiki/Voronoi_diagram
/// \see http://www.cs.hmc.edu/~mbrubeck/voronoi.html
namespace Voronoi
{

/// Notation for working with points
struct Point
{
   Point(double x_ = 0.0, double y_ = 0.0) throw()
      :x(x_),
       y(y_)
   {
   }

   double x;
   double y;
};

/// Voronoi edge
struct Edge
{
   // voronoi edge points
   Point start;
   Point end;

   // delaunay line points
   Point d0;
   Point d1;

//   Point center; ///< index of center point
};

/// sorts edges by angle to given center point
struct EdgeSorter
{
   EdgeSorter(const Vector2d& center)
      :m_center(center)
   {
   }

   bool operator()(const Edge& lhs, const Edge& rhs) const throw()
   {
      Vector2d lhv = m_center - Vector2d(lhs.start.x, lhs.start.y);
      Vector2d rhv = m_center - Vector2d(rhs.start.x, rhs.start.y);

      return lhv.PolarAngle() < rhv.PolarAngle();
   }

   Vector2d m_center;
};

/// \brief Voronoi diagram
class Diagram
{
public:
   /// \brief ctor; constructs a voroni diagram
   Diagram(const std::vector<Vector2d>& vecPoints, const Vector2d& vBounds);
   /// dtor
   ~Diagram();

   // get methods

   /// returns edges for a given site index
   void GetEdgesBySite(size_t index, std::vector<Edge>& vecEdges);

   /// returns all edges (no double edges)
   void GetAllEdges(std::vector<Edge>& vecAllEdges);

   /// returns region points (polygon points in ccw direction) for a given site index
   void Region(size_t index, std::vector<Vector2d>& vecRegionPoints);

   // actions

   /// outputs voronoi diagram as .svg file
   void OutputSvg(LPCTSTR pszFilename, bool bDrawArrows = false, bool bDrawDelaunay = false);

private:
   // Nested classes

   // Arc, Event, and segment datatypes
   struct Arc;
   struct Segment;

   struct Site
   {
      Point p;
      size_t index;
   };

   struct Event
   {
      double x;
      Point p;
      Arc* a;
      bool valid;

      Event(double xx, Point pp, Arc* aa)
         : x(xx), p(pp), a(aa), valid(true)
      {
      }
   };

   typedef std::shared_ptr<Event> EventPtr;

   struct Arc
   {
      const Point p;
      size_t index; ///< point index
      Arc* prev,* next;
      EventPtr e;

      Segment* s0,* s1;

      Arc(Point pp, size_t pindex, Arc* a=0, Arc* b=0)
       : p(pp), index(pindex), prev(a), next(b), s0(0), s1(0)
      {
      }
   };

   /// segment
   struct Segment
   {
      Point start, end;
      bool done;
      bool invalid;
      size_t index0, index1; // indexes of center points of adjacent polygons

      Segment(Point p)
         :start(p),
         end(0,0),
         done(false),
         invalid(false),
         index0(0),
         index1(0)
      {
      }

      /// Set the end point and mark as "done."
      void Finish(Point p)
      {
         if (done)
            return;
         end = p;
         done = true;
      }
   };

   /// "Greater than" comparison, for reverse sorting in priority queue.
   struct GreaterThan
   {
      bool operator()(const Site& a, const Site& b) const throw();
      bool operator()(const EventPtr& a, const EventPtr& b) const throw();
   };

private:
   // Function declarations
   void Process();
   void ProcessPoint();
   void ProcessEvent();
   void FrontInsert(const Point p, size_t index);

   bool Circle(Point a, Point b, Point c, double *x, Point *o);
   void CheckCircleEvent(Arc* i, double x0);

   bool Intersect(Point p, Arc* i, Point *result = 0);
   Point Intersection(Point p0, Point p1, double l);

   void FinishEdges();

   void CombineCollinearSegments();

   void ClipSegments();

   void AddSegment(Segment* pNewSegment);

private:
   /// Bounding box coordinates.
   double m_xmin, m_xmax, m_ymin, m_ymax;

   typedef std::vector<Site> SiteList;
   SiteList m_sites; ///< list of all sites

   /// type to map site index to multiple edges;
   typedef std::multimap<size_t, Segment*> SiteEdgeMultimap;
   SiteEdgeMultimap m_siteEdgeList;

   std::vector<Segment*> m_output;  ///< Array of m_output segments.

   typedef std::priority_queue<Site, std::vector<Site>, GreaterThan> SiteEventPriorityQueue;
   typedef std::priority_queue<EventPtr, std::vector<EventPtr>, GreaterThan> CircleEventPriorityQueue;

   SiteEventPriorityQueue m_points; ///< Site events
   CircleEventPriorityQueue m_events; ///< circle events

   Arc* m_root; ///< First item in the parabolic front linked list.
};

} // namespace Voronoi
