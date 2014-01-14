//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file Voronoi.cpp Voronoi diagram
//

// include
#include "stdafx.h"
#include "Voronoi.hpp"

using namespace Voronoi;

bool Diagram::GreaterThan::operator()(const Site& a, const Site& b) const throw()
{
   return a.p.x==b.p.x ? a.p.y>b.p.y : a.p.x>b.p.x;
}

bool Diagram::GreaterThan::operator()(const EventPtr& a, const EventPtr& b) const throw()
{
   return a->x > b->x;
}

Diagram::Diagram(const std::vector<Vector2d>& vecPoints, const Vector2d& vBounds)
:m_xmin(0.0), m_xmax(vBounds.X()),
 m_ymin(0.0), m_ymax(vBounds.Y()),
 m_root(0)
{
   for (size_t i=0; i<vecPoints.size(); i++)
   {
      Point p(vecPoints[i].X(), vecPoints[i].Y());

      Site s;
      s.p = p;
      s.index = i;
      m_sites.push_back(s);
   }

   {
      std::vector<Site> vecSites(m_sites);
      std::sort(vecSites.begin(), vecSites.end(), GreaterThan());
      m_points = SiteEventPriorityQueue(vecSites.begin(), vecSites.end());
   }

   Process();
}

Diagram::~Diagram()
{
   m_siteEdgeList.clear();

   for (std::vector<Segment*>::const_iterator i = m_output.begin(); i != m_output.end(); ++i)
      delete (*i);

   // TODO delete all nodes
   delete m_root;
}

void Diagram::Process()
{
   // Process the queues; select the top element with smaller x coordinate.
   while (!m_points.empty())
   {
      if (!m_events.empty() && m_events.top()->x <= m_points.top().p.x)
         ProcessEvent();
      else
         ProcessPoint();
   }

   // After all points are processed, do the remaining circle events.
   while (!m_events.empty())
      ProcessEvent();

   FinishEdges(); // Clean up dangling edges.

   CombineCollinearSegments();

   ClipSegments();
}

void Diagram::ProcessPoint()
{
   // Get the next point from the queue.
   Site s = m_points.top();
   m_points.pop();
   Point p = s.p;
   size_t index = s.index;

   // Add a new arc to the parabolic front.
   FrontInsert(p, index);
}

void Diagram::ProcessEvent()
{
   // Get the next event from the queue.
   EventPtr e = m_events.top();
   m_events.pop();

   if (e->valid)
   {
      // Start a new edge.
      Segment* s = new Segment(e->p);

      // Remove the associated arc from the front.
      Arc* a = e->a;

      ATLASSERT(a->prev != NULL);
      if (a->prev)
      {
         a->prev->next = a->next;
         a->prev->s1 = s;
         s->index0 = a->prev->index;
      }
      ATLASSERT(a->next);
      if (a->next)
      {
         a->next->prev = a->prev;
         a->next->s0 = s;
         s->index1 = a->next->index;
      }
      AddSegment(s);

      // Finish the edges before and after a.
      if (a->s0) a->s0->Finish(e->p);
      if (a->s1) a->s1->Finish(e->p);

      // Recheck circle events on either side of p:
      if (a->prev) CheckCircleEvent(a->prev, e->x);
      if (a->next) CheckCircleEvent(a->next, e->x);
   }
// TODO   //e->valid = false;
   e.reset();
}

void Diagram::FrontInsert(const Point p, size_t index)
{
   if (m_root == NULL)
   {
      m_root = new Arc(p, index);
      return;
   }

   // Find the current arc(s) at height p.y (if there are any).
   for (Arc* i = m_root; i; i = i->next)
   {
      Point z, zz;
      if (Intersect(p,i,&z))
      {
         // New parabola intersects arc i.  If necessary, duplicate i.
         if (i->next && !Intersect(p,i->next, &zz))
         {
            i->next->prev = new Arc(i->p, i->index, i, i->next);
            i->next = i->next->prev;
         }
         else
            i->next = new Arc(i->p, i->index, i);
         i->next->s1 = i->s1;

         // Add p between i and i->next.
         i->next->prev = new Arc(p, index, i, i->next);
         i->next = i->next->prev;

         i = i->next; // Now i points to the new arc.

         // Add new half-edges connected to i's endpoints.
         i->prev->s1 = i->s0 = new Segment(z);
         i->s0->index0 = i->prev->index;
         i->s0->index1 = i->index;
         AddSegment(i->s0);

         i->next->s0 = i->s1 = new Segment(z);
         i->s1->index0 = i->next->index;
         i->s1->index1 = i->index;
         AddSegment(i->s1);

         // Check for new circle events around the new arc:
         CheckCircleEvent(i, p.x);
         CheckCircleEvent(i->prev, p.x);
         CheckCircleEvent(i->next, p.x);

         return;
      }
   }

   // Special case: If p never intersects an arc, append it to the list.
   Arc* i;
   for (i = m_root; i->next; i=i->next) ; // Find the last node.

   i->next = new Arc(p, index, i);
   // Insert segment between p and i
   Point start;
   start.x = m_xmin;
   start.y = (i->next->p.y + i->p.y) / 2;
   i->s1 = i->next->s0 = new Segment(start);
   i->s1->index0 = i->next->index;
   i->s1->index1 = i->index;
   AddSegment(i->s1);
}

// Look for a new circle event for arc i.
void Diagram::CheckCircleEvent(Arc* i, double x0)
{
   // Invalidate any old event.
   if (i->e && i->e->x != x0)
      i->e->valid = false;
   i->e.reset();

   if (!i->prev || !i->next)
      return;

   double x;
   Point o;

   if (Circle(i->prev->p, i->p, i->next->p, &x,&o) && x > x0)
   {
      // Create new event.
      //i->e = std::make_shared(new Event(x, o, i));
      i->e.reset(new Event(x, o, i));
      m_events.push(i->e);
   }
}

// Find the rightmost point on the circle through a,b,c.
bool Diagram::Circle(Point a, Point b, Point c, double *x, Point *o)
{
   // Check that bc is a "right turn" from ab.
   if ((b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y) > 0)
      return false;

   // Algorithm from O'Rourke 2ed p. 189.
   double A = b.x - a.x,  B = b.y - a.y,
          C = c.x - a.x,  D = c.y - a.y,
          E = A*(a.x+b.x) + B*(a.y+b.y),
          F = C*(a.x+c.x) + D*(a.y+c.y),
          G = 2*(A*(c.y-b.y) - B*(c.x-b.x));

   if (G == 0) return false;  // Points are co-linear.

   // Point o is the center of the circle.
   o->x = (D*E-B*F)/G;
   o->y = (A*F-C*E)/G;

   // o.x plus radius equals max x coordinate.
   *x = o->x + sqrt( pow(a.x - o->x, 2) + pow(a.y - o->y, 2) );
   return true;
}

// Will a new parabola at point p intersect with Arc i?
bool Diagram::Intersect(Point p, Arc* i, Point *result)
{
   if (i->p.x == p.x) return false;

   double a = 0.0, b = 0.0;

   if (i->prev) // Get the intersection of i->prev, i.
      a = Intersection(i->prev->p, i->p, p.x).y;
   if (i->next) // Get the intersection of i->next, i.
      b = Intersection(i->p, i->next->p, p.x).y;

   if ((!i->prev || a <= p.y) && (!i->next || p.y <= b))
   {
      result->y = p.y;

      result->x = (i->p.x*i->p.x + (i->p.y-result->y)*(i->p.y-result->y) - p.x*p.x)
                / (2*i->p.x - 2*p.x);

      return true;
   }
   return false;
}

// Where do two parabolas intersect?
Point Diagram::Intersection(Point p0, Point p1, double l)
{
   Point res, p = p0;

   double z0 = 2*(p0.x - l);
   double z1 = 2*(p1.x - l);

   if (p0.x == p1.x)
      res.y = (p0.y + p1.y) / 2;
   else if (p1.x == l)
      res.y = p1.y;
   else if (p0.x == l)
   {
      res.y = p0.y;
      p = p1;
   }
   else
   {
      // Use the quadratic formula.
      double a = 1/z0 - 1/z1;
      double b = -2*(p0.y/z0 - p1.y/z1);
      double c = (p0.y*p0.y + p0.x*p0.x - l*l)/z0
               - (p1.y*p1.y + p1.x*p1.x - l*l)/z1;

      res.y = ( -b - sqrt(b*b - 4*a*c) ) / (2*a);
   }
   // Plug back into one of the parabola equations.
   res.x = (p.x*p.x + (p.y-res.y)*(p.y-res.y) - l*l)/(2*p.x-2*l);
   return res;
}

void Diagram::FinishEdges()
{
   // Advance the sweep line so no parabolas can cross the bounding box.
   double l = m_xmax + (m_xmax-m_xmin) + (m_ymax-m_ymin);

   // Extend each remaining segment to the new parabola intersections.
   for (Arc* i = m_root; i->next; i = i->next)
      if (i->s1)
         i->s1->Finish(Intersection(i->p, i->next->p, l*2));
}

// Liang-Barsky function by Daniel White @ http://www.skytopia.com/project/articles/compsci/clipping.html
// This function inputs 8 numbers, and outputs 4 new numbers (plus a boolean value to say whether the clipped line is drawn at all).
//
bool LiangBarsky(double edgeLeft, double edgeRight, double edgeBottom, double edgeTop,   // Define the x/y clipping values for the border.
                 double x0src, double y0src, double x1src, double y1src,                 // Define the start and end points of the line.
                 double& x0clip, double& y0clip, double& x1clip, double& y1clip)         // The output values, so declare these outside.
{
   double t0 = 0.0;    double t1 = 1.0;
   double xdelta = x1src-x0src;
   double ydelta = y1src-y0src;
   double p = 1.0,q = 0.0;

   for (int edge=0; edge<4; edge++)
   {
      // Traverse through left, right, bottom, top edges.
      if (edge==0) {  p = -xdelta;    q = -(edgeLeft-x0src);  }
      if (edge==1) {  p = xdelta;     q =  (edgeRight-x0src); }
      if (edge==2) {  p = -ydelta;    q = -(edgeBottom-y0src);}
      if (edge==3) {  p = ydelta;     q =  (edgeTop-y0src);   }   
      double r = q/p;
      if (p==0 && q<0) return false;   // Don't draw line at all. (parallel line outside)

      if (p<0)
      {
         if (r>t1) return false;         // Don't draw line at all.
         else if (r>t0) t0=r;            // Line is clipped!
      }
      else if (p>0)
      {
         if (r<t0) return false;      // Don't draw line at all.
         else if (r<t1) t1=r;         // Line is clipped!
      }
   }

   x0clip = x0src + t0*xdelta;
   y0clip = y0src + t0*ydelta;
   x1clip = x0src + t1*xdelta;
   y1clip = y0src + t1*ydelta;

   return true;        // (clipped) line is drawn
}

void Diagram::AddSegment(Segment* pNewSegment)
{
   m_output.push_back(pNewSegment);

   m_siteEdgeList.insert(std::make_pair(pNewSegment->index0, pNewSegment));
   m_siteEdgeList.insert(std::make_pair(pNewSegment->index1, pNewSegment));
}

/// clip all segments to bounding rect
/// \see http://www.skytopia.com/project/articles/compsci/clipping.html
void Diagram::ClipSegments()
{
   for (size_t i=0, iMax=m_output.size(); i<iMax;)
   {
      if (m_output[i]->invalid)
      {
         i++;
         continue;
      }

      Point& p0 = m_output[i]->start;
      Point& p1 = m_output[i]->end;

      if (!LiangBarsky(m_xmin, m_xmax, m_ymin, m_ymax,
         p0.x, p0.y, p1.x, p1.y,
         p0.x, p0.y, p1.x, p1.y))
      {
         m_output[i]->invalid = true;
         i++;
      }
      else
      {
         i++;
         // correct possible "-0.000" values
         p0.x = std::max(p0.x, m_xmin);
         p0.x = std::min(p0.x, m_xmax);
         p0.y = std::max(p0.y, m_ymin);
         p0.y = std::min(p0.y, m_ymax);

         p1.x = std::max(p1.x, m_xmin);
         p1.x = std::min(p1.x, m_xmax);
         p1.y = std::max(p1.y, m_ymin);
         p1.y = std::min(p1.y, m_ymax);

         ATLASSERT(p0.x >= m_xmin && p0.x <= m_xmax);
         ATLASSERT(p1.x >= m_xmin && p1.x <= m_xmax);

         ATLASSERT(p0.y >= m_ymin && p0.y <= m_ymax);
         ATLASSERT(p1.y >= m_ymin && p1.y <= m_ymax);
      }
   }
}

bool IsSamePoint(const Point& p0, const Point& p1)
{
   double dx = p0.x-p1.x;
   double dy = p0.y-p1.y;
   return sqrt(dx*dx + dy*dy) < 1e-6;
}

void Diagram::CombineCollinearSegments()
{
   // go through all sites, combining segments that are collinear
   for (size_t iSite=0, iSiteMax=m_sites.size(); iSite<iSiteMax; iSite++)
   {
      SiteEdgeMultimap::const_iterator iterFirst = m_siteEdgeList.find(iSite);

      SiteEdgeMultimap::const_iterator iter = iterFirst;
      for (;iter != m_siteEdgeList.end() && iter->first == iSite; ++iter)
      {
         // don't check first segment, as it has no previous
         if (iter == iterFirst)
            continue;

         Segment& seg1 = *iter->second;
         if (seg1.invalid)
            continue;

         // check previous edges
         SiteEdgeMultimap::const_iterator iter2 = iterFirst;
         for (; iter2 != iter && !seg1.invalid; ++iter2)
         {
            Segment& seg2 = *iter2->second;
            if (seg2.invalid)
               continue;

            // check if the segments share the same center point
            if ((seg1.index0 == seg2.index0 &&
                 seg1.index1 == seg2.index1) ||
                 (seg1.index0 == seg2.index1 &&
                 seg1.index1 == seg2.index0))
            {
               // same start points?
               if (IsSamePoint(seg1.start, seg2.start))
               {
                  // extend seg2 edge at start
                  seg2.start = seg1.end;
                  seg1.invalid = true;
                  continue;
               }

               // we never have these case, so don't check them
               ATLASSERT(false == IsSamePoint(seg1.start, seg2.end));
               ATLASSERT(false == IsSamePoint(seg1.end, seg2.start));
               ATLASSERT(false == IsSamePoint(seg1.end, seg2.end));
            }
         }
      }
   }
}

bool IsCounterClockwiseWinding(const Point& p0, const Point& p1, const Point& p2)
{
   // http://www.cgafaq.info/wiki/Simple_Polygon_Orientation
   // http://www.cgafaq.info/wiki/Polygon_Area
   // calc area: 2 A(P) = ∑[i=0..n-1] (xi yi+1 - yi xi+1)
   // or: 2 A(P) = ∑[i=0..2] (xi yi+1 - yi xi+1)
   // 2 A(P) = (x0 y1 - y0 x1) + (x1 y2 - y1 x2) + (x2 y0 - y2 x0)
   double a =
      (p0.x * p1.y - p0.y * p1.x) +
      (p1.x * p2.y - p1.y * p2.x) +
      (p2.x * p0.y - p2.y * p0.x);

   return a > 0.0;
}

/// \note doesn't clear vecEdges before adding, so can be called multiple times
/// to collect edges
void Diagram::GetEdgesBySite(size_t index, std::vector<Edge>& vecEdges)
{
   ATLASSERT(index < m_sites.size());

   const Point& center = m_sites[index].p;

   SiteEdgeMultimap::const_iterator iter = m_siteEdgeList.find(index);
   for (;iter != m_siteEdgeList.end() && iter->first == index; ++iter)
   {
      Segment& seg = *iter->second;
      if (seg.invalid)
         continue;
/*
      // check previous edges
      for (size_t i=0, iMax=vecEdges.size(); i<iMax; i++)
      {
         if (IsSamePoint(vecEdges[i].start, seg.end))
         {
            // extend edge to segment's start
            vecEdges[i].start = seg.start;
            continue;
         }
         else
         if (IsSamePoint(vecEdges[i].end, seg.start))
         {
            // extend edge to segment's end
            vecEdges[i].end = seg.end;
            continue;
         }
      }
*/
      // add edge
      Edge e;
      // voronoi edge
      e.start = seg.start;
      e.end = seg.end;
      // delaunay edge
      e.d0 = m_sites[seg.index0].p;
      e.d1 = m_sites[seg.index1].p;

      // correct winding
      if (!IsCounterClockwiseWinding(center, e.start, e.end))
         std::swap(e.start, e.end);

      vecEdges.push_back(e);
   }
}

void Diagram::GetAllEdges(std::vector<Edge>& vecAllEdges)
{
   for (size_t i=0, iMax=m_output.size(); i<iMax; i++)
   {
      Segment& seg = *m_output[i];
      if (seg.invalid)
         continue;

      // add edge
      Edge e;
      // voronoi edge
      e.start = seg.start;
      e.end = seg.end;
      // delaunay edge
      e.d0 = m_sites[seg.index0].p;
      e.d1 = m_sites[seg.index1].p;

      vecAllEdges.push_back(e);      
   }
}

void Diagram::Region(size_t index, std::vector<Vector2d>& vecRegionPoints)
{
   ATLASSERT(index < m_sites.size());

   std::vector<Edge> vecEdges;
   GetEdgesBySite(index, vecEdges);

   if (vecEdges.empty())
      return;

   // get center
   const Point& p = m_sites[index].p;
   Vector2d vCenter(p.x, p.y);

   // sort edges by angle they have with center
   std::sort(vecEdges.begin(), vecEdges.end(), EdgeSorter(vCenter));

   for (size_t i=0, iMax=vecEdges.size(); i<iMax; i++)
      vecRegionPoints.push_back(Vector2d(vecEdges[i].start.x, vecEdges[i].start.y));
}

void Diagram::OutputSvg(LPCTSTR pszFilename, bool bDrawArrows, bool bDrawDelaunay)
{
   FILE* fd = NULL;
   errno_t err = _tfopen_s(&fd, pszFilename, _T("wt"));
   if (err != 0 || fd == NULL)
      return;

   _ftprintf(fd,
      _T("<?xml version=\"1.0\" standalone=\"no\"?>\n")
      _T("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n")
      _T("<svg width=\"%u\" height=\"%u\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n"),
      unsigned(m_xmax), unsigned(m_ymax));

   for (SiteList::const_iterator i = m_sites.begin(); i != m_sites.end(); ++i)
   {
      _ftprintf(fd,_T("<circle cx=\"%3.2f\" cy=\"%3.2f\" r=\"5.0\" ")
         _T("style=\"stroke:rgb(255,0,0);stroke-width:2\"/>\n"),
         i->p.x, i->p.y);
   }

   unsigned int uiCount = 0;
   for (std::vector<Segment*>::const_iterator i = m_output.begin(); i != m_output.end(); ++i)
   {
      if ((*i)->invalid)
         continue;

      Point start = (*i)->start;
      Point end = (*i)->end;
      _ftprintf(fd,_T("<line x1=\"%3.2f\" y1=\"%3.2f\" x2=\"%3.2f\" y2=\"%3.2f\" "),
         start.x, start.y, end.x, end.y);

      CString cszStyle;

      cszStyle.Format(_T("style=\"stroke:rgb(%s);stroke-width:%u\"/>\n"),
         uiCount & 1 ? _T("99,99,0") : _T("99,0,99"),
         uiCount & 1 ? 2 : 4
         );

      _ftprintf(fd, cszStyle);

      // paint arrow at the "end" side of segment
      if (bDrawArrows)
      {
         Vector2d v1(end.x-start.x, end.y-start.y);
         v1.Normalize();
         v1.Rotate(155.0);
         v1 *= 20.0;

         {
            _ftprintf(fd,_T("<line x1=\"%3.2f\" y1=\"%3.2f\" x2=\"%3.2f\" y2=\"%3.2f\" "),
               end.x, end.y, end.x+v1.X(), end.y+v1.Y());

            _ftprintf(fd, cszStyle);
         }

         Vector2d v2(end.x-start.x, end.y-start.y);
         v2.Normalize();
         v2.Rotate(-155.0);
         v2 *= 20.0;
         {
            _ftprintf(fd,_T("<line x1=\"%3.2f\" y1=\"%3.2f\" x2=\"%3.2f\" y2=\"%3.2f\" "),
               end.x, end.y, end.x+v2.X(), end.y+v2.Y());

            _ftprintf(fd, cszStyle);
         }
      }

      uiCount++;

      // calc intersection of start/end and p0
      if (bDrawDelaunay)
      {
         Point p0 = m_sites[(*i)->index0].p;

         Vector2d v0(p0.x-start.x, p0.y-start.y),
            v2(end.x-start.x, end.y-start.y);

         Vector2d int1 = Vector2d(start.x, start.y) + (v0.Dot(v2) / v2.Dot(v2)) * v2;

         if (p0.x != 0.0 && p0.y != 0.0)
         {
            _ftprintf(fd,_T("<line x1=\"%3.2f\" y1=\"%3.2f\" x2=\"%3.2f\" y2=\"%3.2f\" style=\"stroke:rgb(0,255,0);stroke-width:2\"/>\n"),
               int1.X(), int1.Y(), p0.x, p0.y);
         }
      }

      // calc intersection of start/end and p1
      if (bDrawDelaunay)
      {
         Point p1 = m_sites[(*i)->index1].p;

         Vector2d v0(p1.x-start.x, p1.y-start.y),
            v2(end.x-start.x, end.y-start.y);

         Vector2d int1 = Vector2d(start.x, start.y) + (v0.Dot(v2) / v2.Dot(v2)) * v2;

         if (p1.x != 0.0 && p1.y != 0.0)
         {
            _ftprintf(fd,_T("<line x1=\"%3.2f\" y1=\"%3.2f\" x2=\"%3.2f\" y2=\"%3.2f\" style=\"stroke:rgb(0,255,0);stroke-width:2\"/>\n"),
               int1.X(), int1.Y(), p1.x, p1.y);
         }
      }

   }
   _ftprintf(fd, _T("</svg>\n"));
   fclose(fd);
}
