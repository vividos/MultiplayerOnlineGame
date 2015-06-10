//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Loader.hpp MilkShape3D loader class
//

// includes
#include "StdAfx.h"
#include "Loader.hpp"
#include "Data.hpp"
#include <ulib/stream/IStream.hpp>
#include <ulib/stream/EndianAwareFilter.hpp>
#include <boost/foreach.hpp>

using namespace MilkShape3D;

namespace
{

const unsigned int c_uiMaxVertices = 65534;  ///< max. number of vertices
const unsigned int c_uiMaxTriangles = 65534; ///< max. number of triangles
const unsigned int c_uiMaxGroups = 255;      ///< max. number of groups
const unsigned int c_uiMaxMaterials = 128;   ///< max. number of materials
const unsigned int c_uiMaxJoints = 128;      ///< max. number of joints

// set 1 byte struct alignment
#pragma pack(push, 1)

/// .ms3d file header
struct Header
{
   char aszId[10];
   int  iVersion;
};

/// header id
static LPCSTR c_szHeaderId = "MS3D000000";

/// default version
const int c_iDefaultVersion = 4;

static_assert(sizeof(Header) == 14, "MS3D Header must be of size 14");


/// vertex flags
enum Flags
{
   flagSelected = 1,
   flagHidden = 2,
   flagSelected2 = 4,
   flagDirty = 8,
   flagIsKey = 16,
};

/// vertex data
struct VertexData
{
   BYTE bFlags;            ///< vertex flags; see enum Flags
   float afVertex[3];      ///< vertex x,y,z
   char cBoneId;           ///< bone id, or c_cBoneIdNone
   BYTE bReferenceCount;   ///< ref count
};

/// default bone id
const char c_cBoneIdNone = -1;

static_assert(sizeof(VertexData) == 15, "VertexData struct must be of size 15");


/// triangle data
struct TriangleData
{
   WORD wFlags;
   WORD awVertexIndices[3];
   float afVertexNormals[3][3];
   float s[3];
   float t[3];
   BYTE bSmoothingGroup; /// 1-32
   BYTE bGroupIndex;
};

static_assert(sizeof(TriangleData) == 70, "TriangleData must be of size 70");

/// group data
struct GroupData
{
   BYTE bFlags;
   CString cszName;
   std::vector<WORD> vecTriangleIndices;
   char cMaterialIndex;
};


// reset struct alignment
#pragma pack(pop)

} // unnamed namespace


void Loader::Load(Stream::IStream& stream)
{
   LoadHeader(stream);
   LoadVertexList(stream);
   LoadTriangleList(stream);
   LoadGroupList(stream);
   LoadMaterialsList(stream);
   LoadAnimationData(stream);
   LoadJointList(stream);
   CheckData();
}

/// \details reads IEEE754 32-bit float value
float Loader::ReadFloat(Stream::IStream& stream)
{
   float fValue = 0.f;

   DWORD dwBytesRead = 0;
   bool bRet = stream.Read(&fValue, sizeof(fValue), dwBytesRead);

   if (!bRet || dwBytesRead != sizeof(fValue))
      throw Exception(_T("error reading float value"), __FILE__, __LINE__);

   // check invalid float values
   if (_isnan(fValue) || !_finite(fValue))
      throw Exception(_T("error reading float value"), __FILE__, __LINE__);

   return fValue;
}

/// \details reads fixed size char string
template <unsigned int uiLen>
CString ReadString(Stream::IStream& stream, LPCTSTR pszExceptionText)
{
   std::array<char, uiLen+1> aName;
   aName[uiLen] = 0;

   DWORD dwBytesRead = 0;
   bool bRet = stream.Read(&aName[0], uiLen, dwBytesRead);

   if (!bRet || dwBytesRead != uiLen)
      throw Exception(pszExceptionText, __FILE__, __LINE__);

   return CString(&aName[0]);
}

void Loader::LoadHeader(Stream::IStream& stream)
{
   Header header;

   const int iSignatureSize = sizeof(header.aszId);

   DWORD dwBytesRead = 0;
   bool bRet = stream.Read(&header.aszId[0], iSignatureSize, dwBytesRead);

   if (!bRet || dwBytesRead != iSignatureSize ||
       strncmp(header.aszId, c_szHeaderId, iSignatureSize) != 0)
      throw Exception(_T("invalid header signature"), __FILE__, __LINE__);

   Stream::EndianAwareFilter filter(stream);
   header.iVersion = static_cast<int>(filter.Read32LE());

   if (header.iVersion != c_iDefaultVersion)
      throw Exception(_T("invalid header version"), __FILE__, __LINE__);
}

void Loader::LoadVertexList(Stream::IStream& stream)
{
   Stream::EndianAwareFilter filter(stream);

   WORD wNumVertices = filter.Read16LE();
   m_data.m_vecVertices.resize(wNumVertices);

   if (wNumVertices >= c_uiMaxVertices)
      throw Exception(_T("invalid number of vertices"), __FILE__, __LINE__);

   for (WORD w = 0; w<wNumVertices; w++)
   {
      Vertex& v = m_data.m_vecVertices[w];

      BYTE bFlags = stream.ReadByte();
      if ((bFlags & ~(flagSelected | flagSelected2 | flagHidden)) != 0)
          throw Exception(_T("invalid vertex flags"), __FILE__, __LINE__);

      float afVertex[3];
      afVertex[0] = ReadFloat(stream);
      afVertex[1] = ReadFloat(stream);
      afVertex[2] = ReadFloat(stream);

      char cBoneId = static_cast<char>(stream.ReadByte());
      /*BYTE bReferenceCount =*/ stream.ReadByte();

      v.m_vPos = Vector3d(afVertex[0], afVertex[1], afVertex[2]);
      v.m_iJointIndex = cBoneId;
   }
}

void Loader::LoadTriangleList(Stream::IStream& stream)
{
   Stream::EndianAwareFilter filter(stream);

   WORD wNumTriangles = filter.Read16LE();
   m_data.m_vecTriangles.resize(wNumTriangles);

   if (wNumTriangles >= c_uiMaxTriangles)
      throw Exception(_T("invalid number of triangles"), __FILE__, __LINE__);

   for (WORD w = 0; w<wNumTriangles; w++)
   {
      TriangleData t;

      t.wFlags = filter.Read16LE();
      if ((t.wFlags & ~(flagSelected | flagSelected2 | flagHidden)) != 0)
          throw Exception(_T("invalid triangle flags"), __FILE__, __LINE__);

      t.awVertexIndices[0] = filter.Read16LE();
      t.awVertexIndices[1] = filter.Read16LE();
      t.awVertexIndices[2] = filter.Read16LE();

      if (t.awVertexIndices[0] >= m_data.m_vecVertices.size() ||
          t.awVertexIndices[1] >= m_data.m_vecVertices.size() ||
          t.awVertexIndices[2] >= m_data.m_vecVertices.size())
         throw Exception(_T("invalid vertex index value"), __FILE__, __LINE__);

      for (int i=0; i<3; i++)
         for (int j=0; j<3; j++)
            t.afVertexNormals[i][j] = ReadFloat(stream);

      for (int i=0; i<3; i++)
         t.s[i] = ReadFloat(stream);
      for (int i=0; i<3; i++)
         t.t[i] = ReadFloat(stream);

      // check bSmoothingGroup
      t.bSmoothingGroup = stream.ReadByte(); /// 1-32
      if (t.bSmoothingGroup < 1 || t.bSmoothingGroup > 32)
          throw Exception(_T("invalid smooth group value"), __FILE__, __LINE__);

      t.bGroupIndex = stream.ReadByte();

      // copy over to Triangle
      MilkShape3D::Triangle& tri = m_data.m_vecTriangles[w];
      for (int i=0; i<3; i++)
      {
         tri.auiVertexIndices[i] = t.awVertexIndices[i];

         tri.aNormals[i] = Vector3d(
            t.afVertexNormals[i][0],
            t.afVertexNormals[i][1],
            t.afVertexNormals[i][2]);

         // reverse t; MilkShape3D stores them in reverse
         tri.aTex[i] = TexCoord2f(t.s[i], /*1.0f -*/ t.t[i]);
      }
   }
}

void Loader::LoadGroupList(Stream::IStream& stream)
{
   Stream::EndianAwareFilter filter(stream);

   WORD wNumGroups = filter.Read16LE();
   m_data.m_vecGroups.resize(wNumGroups);

   if (wNumGroups > c_uiMaxGroups)
      throw Exception(_T("invalid number of groups"), __FILE__, __LINE__);

   for (WORD w = 0; w<wNumGroups; w++)
   {
      GroupData g;

      g.bFlags = stream.ReadByte();
      if ((g.bFlags & ~(flagSelected | flagHidden)) != 0)
         throw Exception(_T("invalid group flags"), __FILE__, __LINE__);

      g.cszName = ReadString<32>(stream, _T("error reading group name"));

      WORD wNumTriangleIndices = filter.Read16LE();
      if (wNumTriangleIndices > 0)
      {
         m_data.m_vecGroups[w].m_vecTriangleIndices.resize(wNumTriangleIndices);

         for (WORD i=0; i<wNumTriangleIndices; i++)
         {
            WORD wIndex = filter.Read16LE();
            if (wIndex >= m_data.m_vecTriangles.size())
               throw Exception(_T("invalid triangle index value"), __FILE__, __LINE__);

            m_data.m_vecGroups[w].m_vecTriangleIndices[i] = wIndex;
         }
      }

      g.cMaterialIndex = static_cast<char>(stream.ReadByte());

      m_data.m_vecGroups[w].m_uiMaterialIndex = g.cMaterialIndex;
   }
}

void Loader::LoadMaterialsList(Stream::IStream& stream)
{
   Stream::EndianAwareFilter filter(stream);

   WORD wNumMaterials = filter.Read16LE();
   m_data.m_vecMaterials.resize(wNumMaterials);

   if (wNumMaterials > c_uiMaxMaterials)
      throw Exception(_T("invalid number of materials"), __FILE__, __LINE__);

   for (WORD w = 0; w<wNumMaterials; w++)
   {
      Material& m = m_data.m_vecMaterials[w];

      m.cszName = ReadString<32>(stream, _T("error reading material name"));

      for (unsigned int i=0; i<4; i++)
         m.afAmbient[i] = ReadFloat(stream);

      for (unsigned int i=0; i<4; i++)
         m.afDiffuse[i] = ReadFloat(stream);

      for (unsigned int i=0; i<4; i++)
         m.afSpecular[i] = ReadFloat(stream);

      for (unsigned int i=0; i<4; i++)
         m.afEmissive[i] = ReadFloat(stream);

      m.fShininess = ReadFloat(stream);
      m.fTransparency = ReadFloat(stream);

      m.bMode = stream.ReadByte();

      m.cszTexture = ReadString<128>(stream, _T("error reading material texture"));
      m.cszAlphaMap = ReadString<128>(stream, _T("error reading material alpha map"));
   }
}

void Loader::LoadAnimationData(Stream::IStream& stream)
{
   m_data.m_animationData.fAnimationFPS = ReadFloat(stream);
   m_data.m_animationData.fCurrentTime = ReadFloat(stream);

   // correct FPS
   if (m_data.m_animationData.fAnimationFPS < 1.0f)
      m_data.m_animationData.fAnimationFPS = 1.0f;

   Stream::EndianAwareFilter filter(stream);

   m_data.m_animationData.iTotalFrames = static_cast<int>(filter.Read32LE());
}

void Loader::LoadJointList(Stream::IStream& stream)
{
   Stream::EndianAwareFilter filter(stream);

   WORD wNumJoints = filter.Read16LE();
   m_data.m_vecJoints.resize(wNumJoints);

   if (wNumJoints > c_uiMaxJoints)
      throw Exception(_T("invalid number of joints"), __FILE__, __LINE__);

   for (WORD w = 0; w<wNumJoints; w++)
   {
      Joint& j = m_data.m_vecJoints[w];

      j.bFlags = stream.ReadByte();
      if ((j.bFlags & ~(flagSelected | flagDirty | flagIsKey)) != 0)
         throw Exception(_T("invalid joint flags"), __FILE__, __LINE__);

      j.cszName = ReadString<32>(stream, _T("error reading joint name"));
      j.cszParentName = ReadString<32>(stream, _T("error reading joint parent name"));

      float x, y, z;
      x = ReadFloat(stream);
      y = ReadFloat(stream);
      z = ReadFloat(stream);
      j.rotation = RotAngle3d(x, y, z);

      x = ReadFloat(stream);
      y = ReadFloat(stream);
      z = ReadFloat(stream);
      j.vPosition = Vector3d(x, y, z);

      WORD wNumKeyFramesRot = filter.Read16LE();
      WORD wNumKeyFramesTrans = filter.Read16LE();

      j.RotationKeys().resize(wNumKeyFramesRot);
      for (WORD w=0; w<wNumKeyFramesRot; w++)
      {
         float fTime = ReadFloat(stream);
         fTime *= m_data.m_animationData.fAnimationFPS;

         float x, y, z;
         x = ReadFloat(stream);
         y = ReadFloat(stream);
         z = ReadFloat(stream);

         j.RotationKeys()[w] = KeyframeRot(fTime, RotAngle3d(x, y, z));
      }

      j.PositionKeys().resize(wNumKeyFramesTrans);
      for (WORD w=0; w<wNumKeyFramesTrans; w++)
      {
         float fTime = ReadFloat(stream);
         fTime *= m_data.m_animationData.fAnimationFPS;

         float x, y, z;
         x = ReadFloat(stream);
         y = ReadFloat(stream);
         z = ReadFloat(stream);

         j.PositionKeys()[w] = KeyframeTrans(fTime, Vector3d(x, y, z));
      }
   }
}

void Loader::CheckData()
{
   // check joint indices
   size_t uiNumJoints = m_data.m_vecJoints.size();
   BOOST_FOREACH(const Vertex& v, m_data.m_vecVertices)
   {
      if (v.m_iJointIndex != -1 && static_cast<size_t>(v.m_iJointIndex) >= uiNumJoints)
         throw Exception(_T("invalid joint index in vertices"), __FILE__, __LINE__);
   }

   size_t uiNumMaterials = m_data.m_vecMaterials.size();
   BOOST_FOREACH(const Group& g, m_data.m_vecGroups)
   {
      if (g.m_uiMaterialIndex >= uiNumMaterials)
         throw Exception(_T("invalid material index in groups"), __FILE__, __LINE__);
   }
}
