//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\Loader.hpp MilkShape3D loader class
//
#pragma once

// forward references
namespace Stream
{
class IStream;
};

namespace MilkShape3D
{
struct Data;

/// \brief loader for .ms3d file format
/// \details only supports loading up to the joint list
class Loader
{
public:
   /// ctor
   Loader(Data& data)
      :m_data(data)
   {
   }

   /// loads file from stream
   void Load(Stream::IStream& stream);

private:
   /// reads float value
   static float ReadFloat(Stream::IStream& stream);

   /// loads header
   void LoadHeader(Stream::IStream& stream);

   /// loads vertices
   void LoadVertexList(Stream::IStream& stream);

   /// loads triangles
   void LoadTriangleList(Stream::IStream& stream);

   /// loads group list
   void LoadGroupList(Stream::IStream& stream);

   /// loads materials list
   void LoadMaterialsList(Stream::IStream& stream);

   /// loads animation data
   void LoadAnimationData(Stream::IStream& stream);

   /// loads joints
   void LoadJointList(Stream::IStream& stream);

   /// checks loaded data
   void CheckData();

private:
   /// model data
   Data& m_data;
};

} // namespace MilkShape3D
