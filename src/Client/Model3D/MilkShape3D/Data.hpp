//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\Data.hpp MilkShape3D data classes
//
#pragma once

// includes
#include "Vector3.hpp"
#include "Matrix4.hpp"
#include "TexCoord2f.hpp"
#include "Texture.hpp"
#include "RangedAppendOnlyIndexBuffer.hpp"
#include <array>

/// \brief MilkShape 3D classes
/// \see http://chumbalum.swissquake.ch/ms3d/ms3dspec.txt
namespace MilkShape3D
{

/// vertex infos
struct Vertex
{
   /// ctor
   Vertex()
      :m_iJointIndex(-1)
   {
   }

   /// vertex pos
   Vector3d m_vPos;

   /// joint the vertex belongs to
   int m_iJointIndex;
};

/// triangle infos
struct Triangle
{
   /// ctor
   Triangle() throw()
   {
      std::uninitialized_fill(auiVertexIndices, auiVertexIndices+3, 0);
   }

   /// indices into vertex list
   size_t auiVertexIndices[3];

   /// normals for vertices
   Vector3d aNormals[3];

   /// texture coordinates for vertices
   TexCoord2f aTex[3];
};

/// group infos
/// \details a group is a collection of all triangles with the same material
struct Group
{
   /// triangle indices for this group
   std::vector<size_t> m_vecTriangleIndices;

   /// material index
   size_t m_uiMaterialIndex;
};

/// material data
struct Material
{
   CString cszName;     ///< material name
   float afAmbient[4];  ///< ambient coefficients
   float afDiffuse[4];  ///< diffuse coefficients
   float afSpecular[4]; ///< specular coefficients
   float afEmissive[4]; ///< emissive coefficients
   float fShininess;    ///< shininess value
   float fTransparency; ///< transparency value
   BYTE bMode;          ///< mode flags
   CString cszTexture;  ///< material texture name
   CString cszAlphaMap; ///< alpha map texture name

   /// uses material in OpenGL
   void Use() const;
};

/// animation data
struct AnimationData
{
   /// ctor
   AnimationData()
      :fAnimationFPS(1.0),
       fCurrentTime(0.0),
       iTotalFrames(1)
   {
   }

   /// animation frames per second
   float fAnimationFPS;
   /// current animation time value
   float fCurrentTime;
   /// total number of frames
   int iTotalFrames;
};


/// rotation angles for x, y and z axis
class RotAngle3d
{
public:
   /// ctor
   RotAngle3d(double dAngleX = 0.0, double dAngleY = 0.0, double dAngleZ = 0.0)
   {
      m_aData[0] = dAngleX;
      m_aData[1] = dAngleY;
      m_aData[2] = dAngleZ;
   }

   double X() const throw() { return m_aData[0]; } ///< returns x axis rotation angle
   double Y() const throw() { return m_aData[1]; } ///< returns y axis rotation angle
   double Z() const throw() { return m_aData[2]; } ///< returns z axis rotation angle

   /// returns raw data values
   const double* Data() const throw() { return m_aData.data(); }

private:
   /// angles
   std::array<double,3> m_aData;
};

static_assert(sizeof(RotAngle3d) == 3*sizeof(double), "RotAngle3d struct must have the size of 3 doubles");

/// rotation keyframe infos
class KeyframeRot
{
public:
   /// default ctor
   KeyframeRot(double dTime = 0.0) throw()
      :m_dTime(dTime)
   {
   }
   /// ctor
   KeyframeRot(double dTime, RotAngle3d key) throw()
      :m_dTime(dTime),
       m_key(key)
   {
   }

   /// returns time value
   double Time() const throw() { return m_dTime; }

   /// returns rotation key
   const RotAngle3d& Key() const throw() { return m_key; }

private:
   double m_dTime;   ///< time value
   RotAngle3d m_key; ///< rotation value
};


/// translation keyframe infos
class KeyframeTrans
{
public:
   /// default ctor
   KeyframeTrans(double dTime = 0.0) throw()
      :m_dTime(dTime)
   {
   }
   /// ctor
   KeyframeTrans(double dTime, const Vector3d& vKey) throw()
      :m_dTime(dTime),
       m_vKey(vKey)
   {
   }

   /// returns time value
   double Time() const throw() { return m_dTime; }

   /// returns translation value
   const Vector3d& Key() const throw() { return m_vKey; }

private:
   double m_dTime;   ///< time value
   Vector3d m_vKey;  ///< translation value
};

/// tangent infos
class Tangent
{
public:
   /// default ctor
   Tangent() throw()
   {
   }
   /// ctor
   Tangent(const Vector3d& vTangentIn, const Vector3d& vTangentOut) throw()
      :m_vTangentIn(vTangentIn),
       m_vTangentOut(vTangentOut)
   {
   }

   /// returns in tangent
   const Vector3d& TangentIn() const throw() { return m_vTangentIn; }

   /// returns out tangent
   const Vector3d& TangentOut() const throw() { return m_vTangentOut; }

private:
   Vector3d m_vTangentIn;  ///< in tangent
   Vector3d m_vTangentOut; ///< out tangent
};


/// joint data
class Joint
{
public:
   /// ctor
   Joint() throw()
      :bFlags(0),
       m_iParentIndex(-1)
   {
   }

   /// parent index, or -1 when none
   int ParentIndex() const throw() { return m_iParentIndex; }

   /// sets new parent index
   void ParentIndex(int iParentIndex) throw()
   {
      ATLASSERT(iParentIndex >= -1);
      m_iParentIndex = iParentIndex;
   }

   const Matrix4d& LocalSkeletonMatrix() const throw() { return m_matLocalSkeleton; } ///< returns local skeleton matrix; const-version
         Matrix4d& LocalSkeletonMatrix()       throw() { return m_matLocalSkeleton; } ///< returns local skeleton matrix

   const Matrix4d& GlobalSkeletonMatrix() const throw() { return m_matGlobalSkeleton; } ///< returns global skeleton matrix; const-version
         Matrix4d& GlobalSkeletonMatrix()       throw() { return m_matGlobalSkeleton; } ///< returns global skeleton matrix

   const std::vector<KeyframeRot>& RotationKeys() const throw() { return m_vecRotationKeys; } ///< returns list of rotation keys; const-version
         std::vector<KeyframeRot>& RotationKeys()       throw() { return m_vecRotationKeys; } ///< returns list of rotation keys

   const std::vector<KeyframeTrans>& PositionKeys() const throw() { return m_vecPositionKeys; } ///< returns list of translation keys; const-version
         std::vector<KeyframeTrans>& PositionKeys()       throw() { return m_vecPositionKeys; } ///< returns list of translation keys

   const std::vector<Tangent>& Tangents() const throw() { return m_vecTangents; } ///< returns list of tangents; const-version
         std::vector<Tangent>& Tangents()       throw() { return m_vecTangents; } ///< returns list of tangents

public:
   /// joint flags
   BYTE bFlags;

   /// joint name
   CString cszName;

   /// parent joint name
   CString cszParentName;

   /// base rotation for joint
   RotAngle3d rotation;

   /// base translation for joint
   Vector3d vPosition;

private:
   /// parent index
   int m_iParentIndex;

   /// rotation keys for all keyframes
   std::vector<KeyframeRot> m_vecRotationKeys;

   /// translation keys for all keyframes
   std::vector<KeyframeTrans> m_vecPositionKeys;

   /// tangents for all keyframes
   std::vector<Tangent> m_vecTangents;

   /// local skeleton matrix
   Matrix4d m_matLocalSkeleton;

   /// global skeleton matrix
   Matrix4d m_matGlobalSkeleton;
};


/// MilkShape3D model data
struct Data: public boost::noncopyable
{
   std::vector<Vertex> m_vecVertices;     ///< list of vertices
   std::vector<Triangle> m_vecTriangles;  ///< list of triangles
   std::vector<Group> m_vecGroups;        ///< list of groups
   std::vector<Material> m_vecMaterials;  ///< list of materials
   AnimationData m_animationData;         ///< animation data
   std::vector<Joint> m_vecJoints;        ///< list of joints
};

} // namespace MilkShape3D
