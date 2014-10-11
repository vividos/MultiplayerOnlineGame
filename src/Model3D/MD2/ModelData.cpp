//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MD2\ModelData.cpp MD2 model data
//

// includes
#include "stdafx.h"
#include "ModelData.hpp"
#include <ulib/stream/EndianAwareFilter.hpp>

namespace
{
#pragma pack(push)
#pragma pack(1)

   /// md2 header
   struct Header
   {
      unsigned int     ident;              ///< magic number. must be equal to "IDP2"
      unsigned int     version;            ///< md2 version. must be equal to 8

      unsigned int     skinwidth;          ///< width of the texture
      unsigned int     skinheight;         ///< height of the texture
      unsigned int     framesize;          ///< size of one frame in bytes

      unsigned int     num_skins;          ///< number of textures
      unsigned int     num_xyz;            ///< number of vertices
      unsigned int     num_st;             ///< number of texture coordinates
      unsigned int     num_tris;           ///< number of triangles
      unsigned int     num_glcmds;         ///< number of opengl commands
      unsigned int     num_frames;         ///< total number of frames

      unsigned int     ofs_skins;          ///< offset to skin names (64 bytes each)
      unsigned int     ofs_st;             ///< offset to s-t texture coordinates
      unsigned int     ofs_tris;           ///< offset to triangles
      unsigned int     ofs_frames;         ///< offset to frame data
      unsigned int     ofs_glcmds;         ///< offset to opengl commands
      unsigned int     ofs_end;            ///< offset to end of file

      void Read(Stream::EndianAwareFilter& filter)
      {
         ident = filter.Read32LE();
         version = filter.Read32LE();

         skinwidth = filter.Read32LE();
         skinheight = filter.Read32LE();
         framesize = filter.Read32LE();

         num_skins = filter.Read32LE();
         num_xyz = filter.Read32LE();
         num_st = filter.Read32LE();
         num_tris = filter.Read32LE();
         num_glcmds = filter.Read32LE();
         num_frames = filter.Read32LE();

         ofs_skins = filter.Read32LE();
         ofs_st = filter.Read32LE();
         ofs_tris = filter.Read32LE();
         ofs_frames = filter.Read32LE();
         ofs_glcmds = filter.Read32LE();
         ofs_end = filter.Read32LE();
      }
   };

   /// vertex
   struct Vertex
   {
      unsigned char v[3];                ///< compressed vertex (x, y, z) coordinates
      unsigned char lightnormalindex;    ///< index to a normal vector for the lighting
   };

   /// frame
   struct Frame
   {
      float     scale[3];       ///< scale values
      float     translate[3];   ///< translation vector
      char      name[16];       ///< frame name
      Vertex    verts[1];       ///< first vertex of this frame
   };

   /// magic number "IDP2" or 844121161
   const unsigned int c_uiMagicNumber = (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I');

   /// model version
   const unsigned int c_uiVersion = 8;

#pragma pack(pop)
} // unnamed namespace

/// \see http://tfc.duke.free.fr/old/models/md2.htm
void MD2::ModelData::Load(Stream::IStream& stream)
{
   Stream::EndianAwareFilter filter(stream);

   Header header = { 0 };
   header.Read(filter);

   if ((header.ident != c_uiMagicNumber) && (header.version != c_uiVersion))
      throw Exception(_T("invalid MD2 signature"), __FILE__, __LINE__);

   // initialize member variables
   m_iNumFrames = header.num_frames;
   m_iNumXyz = header.num_xyz;
   m_iNumGlcmds = header.num_glcmds;

   // allocate memory
   m_vecVertices.resize(header.num_xyz * header.num_frames);
   m_vecGlcmds.resize(header.num_glcmds);
   m_vecLightNormals.resize(header.num_xyz * header.num_frames);
   m_vecMinBounds.resize(header.num_frames);
   m_vecMaxBounds.resize(header.num_frames);

   /////////////////////////////////////////////
   //          reading file data

   std::vector<char> buffer(header.num_frames * header.framesize);

   DWORD dwBytesRead = 0;

   // TODO read frame endian-independent
   // read frame data...
   stream.Seek(header.ofs_frames, Stream::IStream::seekBegin);
   stream.Read(&buffer[0], buffer.size(), dwBytesRead);

   if (buffer.size() != dwBytesRead)
      throw Exception(_T("invalid MD2 content"), __FILE__, __LINE__);

   // read opengl commands...
   stream.Seek(header.ofs_glcmds, Stream::IStream::seekBegin);
   for (size_t i = 0; i < m_vecGlcmds.size(); i++)
      m_vecGlcmds[i] = static_cast<int>(filter.Read32LE());

   /////////////////////////////////////////////
   // vertex array initialization

   for (unsigned int numframe = 0; numframe < header.num_frames; numframe++)
   {
      // get frame struct for this
      Frame* frame = reinterpret_cast<Frame*>(&buffer[header.framesize * numframe]);

      //ATLTRACE(_T("loading frame %u, [%-16hs]\n"), numframe, frame->name);

      Vector3d* ptrverts = &m_vecVertices[header.num_xyz * numframe];
      unsigned int* ptrnormals = &m_vecLightNormals[header.num_xyz * numframe];

      Vector3d vMinBound, vMaxBound;

      for (unsigned int i = 0; i < header.num_xyz; i++)
      {
         Vector3d& v = ptrverts[i];
         v = Vector3d(
            (frame->verts[i].v[0] * frame->scale[0]) + frame->translate[0],
            (frame->verts[i].v[1] * frame->scale[1]) + frame->translate[1],
            (frame->verts[i].v[2] * frame->scale[2]) + frame->translate[2]);

         ptrnormals[i] = frame->verts[i].lightnormalindex;

         // get min and max bounds
         vMinBound.X(std::min(v.X(), vMinBound.X()));
         vMinBound.Y(std::min(v.Y(), vMinBound.Y()));
         vMinBound.Z(std::min(v.Z(), vMinBound.Z()));

         vMaxBound.X(std::max(v.X(), vMaxBound.X()));
         vMaxBound.Y(std::max(v.Y(), vMaxBound.Y()));
         vMaxBound.Z(std::max(v.Z(), vMaxBound.Z()));
      }

      m_vecMinBounds[numframe] = vMinBound;
      m_vecMaxBounds[numframe] = vMaxBound;
   }
}
