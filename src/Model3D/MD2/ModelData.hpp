//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MD2\ModelData.hpp MD2 model data
//
#pragma once

// includes
#include "Vector3d.hpp"
#include "DisplayList.hpp"
#include <deque>

// forward references
namespace Stream
{
   class IStream;
}

namespace MD2
{
#pragma pack(push)
#pragma pack(1)

   /// opengl command
   struct GLCommand
   {
      float s;    ///< S texture coord.
      float t;    ///< T texture coord.
      int index;  ///< Vertex index
   };

   static_assert(sizeof(GLCommand) == 2 * sizeof(float) + sizeof(int), "GLCommand must be same size as 2x float and 1x int");
   static_assert(sizeof(GLCommand) == 3 * sizeof(int), "GLCommand must be same size as 3x int");

#pragma pack(pop)


/// vertex and texture data for a model
struct ModelData
{
public:
   /// ctor
   ModelData()
      :m_iNumFrames(0),
      m_iNumXyz(0),
      m_iNumGlcmds(0),
      m_uiDisplayListPolycount(0)
   {
   }

   /// loads model data
   void Load(Stream::IStream& stream);

   /// number of frames in data
   int m_iNumFrames;

   /// number of vertices
   int m_iNumXyz;

   /// number of OpenGL commands
   int m_iNumGlcmds;

   /// vertices
   std::vector<Vector3d> m_vecVertices;

   /// OpenGL commands
   std::vector<int> m_vecGlcmds;

   /// light normals indices
   std::vector<unsigned int> m_vecLightNormals;

   /// minimum bounds of model, per frame
   std::vector<Vector3d> m_vecMinBounds;

   /// maximum bounds of model, per frame
   std::vector<Vector3d> m_vecMaxBounds;

   // extra data while rendering

   /// prepared display lists
   OpenGL::DisplayListArray m_frameDisplayLists;

   /// polycount for display list
   unsigned int m_uiDisplayListPolycount;

   /// array with info which display lists are already prepared
   std::deque<bool> m_deqDisplayListPrepared;
};

} // namespace MD2
