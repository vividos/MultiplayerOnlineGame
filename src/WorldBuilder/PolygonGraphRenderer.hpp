//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PolygonGraphRenderer.hpp Render class for polygon graph
//
#pragma once

// includes
#include "IRenderable.hpp"
#include "DisplayList.hpp"

namespace PolygonGraph
{
class Graph;
}

/// renderer for PolygonGraph::Graph class
class PolygonGraphRenderer : public IRenderable
{
public:
   /// ctor
   PolygonGraphRenderer(const PolygonGraph::Graph& graph) throw();

   /// dtor
   virtual ~PolygonGraphRenderer() throw() {}

   // virtual methods from IRenderable

   virtual bool IsPrepareNeeded() const override { return false; }
   virtual bool IsUploadNeeded() const override { return true; }
   virtual void Prepare() override {}
   virtual void Upload() override;
   virtual void Render(RenderOptions& options) override;
   virtual void Done() override {}

private:
   /// prepares edge lines
   void PrepareEdges();

   /// prepares polygons
   void PreparePolygons();

private:
   /// polygon graph
   const PolygonGraph::Graph& m_graph;

   /// display list for edges
   OpenGL::DisplayList m_displayListEdges;

   /// display list for polygons
   OpenGL::DisplayList m_displayListPolygons;
};
