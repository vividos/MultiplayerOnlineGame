//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TrianglesCompiler.cpp triangles compiler base class
//

// includes
#include "StdAfx.h"
#include "TrianglesCompiler.hpp"
#include "IActiveVertexMap.hpp"

using Terrain::Reduce::TrianglesCompiler;

TrianglesCompiler::TrianglesCompiler(Terrain::Reduce::IActiveVertexMap& activeVertexMap)
:m_activeVertexMap(activeVertexMap)
{
}

void TrianglesCompiler::Compile(unsigned int uiSize)
{
   CompileBlock(0, 0, uiSize);
}

void TrianglesCompiler::CompileTriangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3)
{
   CompileVertex(x3, y3);
   CompileVertex(x2, y2);
   CompileVertex(x1, y1);
}

void TrianglesCompiler::CompileFan(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, unsigned int x4, unsigned int y4, unsigned int x5, unsigned int y5)
{
   CompileVertex(x1, y1);
   CompileVertex(x5, y5);
   CompileVertex(x4, y4);

   CompileVertex(x1, y1);
   CompileVertex(x4, y4);
   CompileVertex(x3, y3);

   CompileVertex(x1, y1);
   CompileVertex(x3, y3);
   CompileVertex(x2, y2);

   // was:
   //CompileVertex(x1, y1);
   //CompileVertex(x5, y5);
   //CompileVertex(x4, y4);
   //CompileVertex(x3, y3);
   //CompileVertex(x2, y2);
}

void TrianglesCompiler::CompileFan(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, unsigned int x4, unsigned int y4, unsigned int x5, unsigned int y5, unsigned int x6, unsigned int y6)
{
   CompileVertex(x1, y1);
   CompileVertex(x6, y6);
   CompileVertex(x5, y5);

   // use 3-vertex fan function
   CompileFan(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5);

   // was:
   //CompileVertex(x1, y1);
   //CompileVertex(x6, y6);
   //CompileVertex(x5, y5);
   //CompileVertex(x4, y4);
   //CompileVertex(x3, y3);
   //CompileVertex(x2, y2);
}

void TrianglesCompiler::CompileStrip(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, unsigned int x4, unsigned int y4)
{
   CompileVertex(x1, y1);
   CompileVertex(x2, y2);
   CompileVertex(x3, y3);

   CompileVertex(x3, y3);
   CompileVertex(x2, y2);
   CompileVertex(x4, y4);

   // was:
   //CompileVertex(x1, y1);
   //CompileVertex(x2, y2);
   //CompileVertex(x3, y3);
   //CompileVertex(x4, y4);
}

/// \brief compiles a single block
/// \param x start coordinate x
/// \param y start coordinate y
/// \param size size of block; block is assumed to be square
/// \author Shamus Young http://www.shamusyoung.com/twentysidedtale/?p=141
/// \author Michael Fink modified to use index buffer object (IBO)
/// \details
/// \verbatim
/// North                 N
/// *-------*           *---+---*           *---*---*     *---+---*
/// |\      |           |\     /|           |\Nl|Nr/|     |   |   |
/// | \ Sup |           | \   / |           | \ | / |     | A | B |
/// |  \    |           |  \ /  |           |Wr\|/El|     |   |   |
/// |   \   |       West+   *   +East      W*---*---*E    *---+---*
/// |    \  |           |  / \  |           |Wl/|\Er|     |   |   |
/// | Inf \ |           | /   \ |           | / | \ |     | C | D |
/// |      \|           |/     \|           |/Sr|Sl\|     |   |   |
/// *-------*           *---+---*           *---*---*     *---*---*
/// South                 S
///
/// Figure a            Figure b            Figure c      Figure d
/// \endverbatim
/// This takes a single quadtree block and decides how to divide it for rendering.
/// If the center point in not included in the mesh (or if there IS no center
/// because we are at the lowest level of the tree), then the block is simply
/// cut into two triangles. (Figure a)
///
/// If the center point is active, but none of the edges, the block is cut into
/// four triangles.  (Fig. b)  If the edges are active, then the block is cut
/// into a combination of smaller triangles (Fig. c) and sub-blocks (Fig. d).
///
void TrianglesCompiler::CompileBlock(unsigned int x, unsigned int y, unsigned int size)
{
   // Define the shape of this block.  x and y are the upper-left(Northwest)
   // origin, xc and yc define the center, and x2, y2 mark the lower-right
   // (Southeast) corner, and next_size is half the size of this block.
   unsigned int next_size = size / 2;
   unsigned int x2 = x + size;
   unsigned int y2 = y + size;
   unsigned int xc = x + next_size;
   unsigned int yc = y + next_size;

   // If this is the smallest block, or the center is inactive, then just
   // Cut into two triangles as shown in Figure a
   if (size == 1 || !IsActive(xc, yc))
   {
      CompileStrip(x, y, x, y2, x2, y, x2, y2);
      return;
   }

   // If the edges are inactive, we need 4 triangles(fig b)
   if (!IsActive(xc, y) &&
       !IsActive(xc, y2) &&
       !IsActive(x, yc) &&
       !IsActive(x2, yc))
   {
      CompileFan(xc, yc, x, y, x2, y, x2, y2, x, y2, x, y);
      return;
   }

   // If the top & bottom edges are inactive, it is impossible to have
   // sub-blocks, so we can make a single fan
   if (!IsActive(xc, y) && !IsActive(xc, y2))
   {
      CompileVertex(xc, yc);
      CompileVertex(x, y);
      if (IsActive(x, yc))
      {
         CompileVertex(x, yc); // W

         CompileVertex(xc, yc);
         CompileVertex(x, yc);
      }
      CompileVertex(x, y2);

      CompileVertex(xc, yc);
      CompileVertex(x, y2);
      CompileVertex(x2, y2);

      CompileVertex(xc, yc);
      CompileVertex(x2, y2);
      if (IsActive(x2, yc))
      {
         CompileVertex(x2, yc); //E

         CompileVertex(xc, yc);
         CompileVertex(x2, yc);
      }
      CompileVertex(x2, y);

      CompileVertex(xc, yc);
      CompileVertex(x2, y);
      CompileVertex(x, y);
      return;
   }

   // If the left & right edges are inactive, it is impossible to have
   // sub-blocks, so we can make a single fan
   if (!IsActive(x, yc) && !IsActive(x2, yc))
   {
      CompileVertex(xc, yc);
      CompileVertex(x, y);
      CompileVertex(x, y2);

      CompileVertex(xc, yc);
      CompileVertex(x, y2);
      if (IsActive(xc, y2))
      {
         CompileVertex(xc, y2); // S

         CompileVertex(xc, yc);
         CompileVertex(xc, y2);
      }
      CompileVertex(x2, y2);

      CompileVertex(xc, yc);
      CompileVertex(x2, y2);
      CompileVertex(x2, y);

      CompileVertex(xc, yc);
      CompileVertex(x2, y);
      if (IsActive(xc, y))
      {
         CompileVertex(xc, y); // N

         CompileVertex(xc, yc);
         CompileVertex(xc, y);
      }
      CompileVertex(x, y);
      return;
   }

   // None of the other tests worked, which means this block is a combination
   // of triangle strips and sub-blocks. Brace yourself, this is not for the timid.
   // the first step is to find out which triangles we need
   if (!IsActive(xc, y)) // is the top edge inactive?
   {
      // left and right edge active?
      if (IsActive(x, yc) && IsActive(x2, yc))
      {
         CompileFan(xc, yc, x, yc, x, y, x2, y, x2, yc);
      }
      else
      {
         // either left or right edge is inactive
         CompileFanCond(xc, yc,
            IsActive(x2, yc), // EL
            x2, yc,
            x2, y,
            x, y,
            IsActive(x, yc), // WR
            x, yc);
      }
   }

   if (!IsActive(xc, y2)) // is the bottom edge inactive?
   {
      // top and bottom edge active?
      if (IsActive(x, yc) && IsActive(x2, yc))
      {
         CompileFan(xc, yc, x2, yc, x2, y2, x, y2, x, yc);
      }
      else
      {
         CompileFanCond(xc, yc,
            IsActive(x, yc),
            x, yc,
            x, y2,
            x2, y2,
            IsActive(x2, yc),
            x2, yc);
      }
   }

   if (!IsActive(x, yc)) // is the left edge inactive?
   {
      // top and bottom edge active?
      if (IsActive(xc, y) && IsActive(xc, y2))
      {
         CompileFan(xc, yc, xc, y2, x, y2, x, y, xc, y);
      }
      else
      {
         CompileFanCond(xc, yc,
            IsActive(xc, y), // NL
            xc, y,
            x, y,
            x, y2,
            IsActive(xc, y2), // SR
            xc, y2);
      }
   }

   if (!IsActive(x2, yc)) // right edge inactive?
   {
      if (IsActive(xc, y) && IsActive(xc, y2))
      {
         CompileFan(xc, yc, xc, y, x2, y, x2, y2, xc, y2);
      }
      else
      {
         CompileFanCond(xc, yc,
            IsActive(xc, y2), // SL
            xc, y2,
            x2, y2,
            x2, y,
            IsActive(xc, y), // NR
            xc, y);
      }
   }

   // now that the various triangles have been added, we add the
   // various sub-blocks.  This is recursive.
   if (IsActive(xc, y) && IsActive(x, yc))
      CompileBlock(x, y, next_size); // Sub-block A
   if (IsActive(xc, y) && IsActive(x2, yc))
      CompileBlock(x + next_size, y, next_size); // Sub-block B
   if (IsActive(x, yc) && IsActive(xc, y2))
      CompileBlock(x, y + next_size, next_size); // Sub-block C
   if (IsActive(x2, yc) && IsActive(xc, y2))
      CompileBlock(x + next_size, y + next_size, next_size); // Sub-block D
}

void TrianglesCompiler::CompileFanCond(
   unsigned int x1, unsigned int y1,
   bool bActive2,
   unsigned int x2, unsigned int y2,
   unsigned int x3, unsigned int y3,
   unsigned int x4, unsigned int y4,
   bool bActive5,
   unsigned int x5, unsigned int y5)
{
   // if (bActive2) then [1 2 3], [1 3 4] else [1 3 4]
   // if (bActive5) then [1 4 5]

   if (bActive2)
   {
      CompileVertex(x1, y1);
      CompileVertex(x2, y2);
      CompileVertex(x3, y3);
   }

   CompileVertex(x1, y1);
   CompileVertex(x3, y3);
   CompileVertex(x4, y4);

   if (bActive5)
   {
      CompileVertex(x1, y1);
      CompileVertex(x4, y4);
      CompileVertex(x5, y5);
   }
}

bool TrianglesCompiler::IsActive(unsigned int x, unsigned int y) const throw()
{
   return m_activeVertexMap.IsActive(x, y);
}
