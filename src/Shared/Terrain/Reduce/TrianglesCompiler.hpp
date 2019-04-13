//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TrianglesCompiler.hpp triangles compiler base class
//
#pragma once

namespace Terrain
{
namespace Reduce
{
class IActiveVertexMap;

class TrianglesCompiler
{
public:
   /// ctor
   TrianglesCompiler(IActiveVertexMap& activeVertexMap);
   /// dtor
   virtual ~TrianglesCompiler() {}

   /// compiles triangles by given active vertex map
   void Compile(unsigned int uiSize);

private:
   /// adds a vertex to the vertex list; must implement this in derived classes
   virtual void CompileVertex(unsigned int x, unsigned int y) = 0;

   /// adds a triangle
   void CompileTriangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3);

   /// adds a triangle fan
   void CompileFan(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, unsigned int x4, unsigned int y4, unsigned int x5, unsigned int y5);

   /// adds a triangle fan
   void CompileFan(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, unsigned int x4, unsigned int y4, unsigned int x5, unsigned int y5, unsigned int x6, unsigned int y6);

   /// adds a triangle strip
   void CompileStrip(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, unsigned int x4, unsigned int y4);

   /// compiles a block
   void CompileBlock(unsigned int x, unsigned int y, unsigned int size);

   /// conditionally builds a fan with 2, 3 or 4 edge points
   void CompileFanCond(
      unsigned int x1, unsigned int y1,
      bool bActive2,
      unsigned int x2, unsigned int y2,
      unsigned int x3, unsigned int y3,
      unsigned int x4, unsigned int y4,
      bool bActive5,
      unsigned int x5, unsigned int y5);

   /// returns if point at coordinate is active
   bool IsActive(unsigned int x, unsigned int y) const;

private:
   /// active vertex map
   IActiveVertexMap& m_activeVertexMap;
};

} // namespace Reduce
} // namespace Terrain
