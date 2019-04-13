//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\AnimatedModel3d.cpp MilkShape3D animated model
//

// includes
#include "StdAfx.h"
#include "AnimatedModel3d.hpp"
#include "RenderOptions.hpp"
#include "Quaternion4.hpp"
#include "ModelDisplayState.hpp"
#include <functional>
#include <boost/foreach.hpp>

/// creates rotation matrix from rotation angles
void AngleMatrix(const MilkShape3D::RotAngle3d& rotation, Matrix4d& matrix)
{
   double angle = rotation.Z();
   double sy = sin(angle);
   double cy = cos(angle);
   angle = rotation.Y();
   double sp = sin(angle);
   double cp = cos(angle);
   angle = rotation.X();
   double sr = sin(angle);
   double cr = cos(angle);

   // matrix = (Z * Y) * X
   matrix[0][0] = cp*cy;
   matrix[1][0] = cp*sy;
   matrix[2][0] = -sp;
   matrix[0][1] = sr*sp*cy+cr*-sy;
   matrix[1][1] = sr*sp*sy+cr*cy;
   matrix[2][1] = sr*cp;
   matrix[0][2] = (cr*sp*cy+-sr*-sy);
   matrix[1][2] = (cr*sp*sy+-sr*cy);
   matrix[2][2] = cr*cp;
   matrix[0][3] = 0.0;
   matrix[1][3] = 0.0;
   matrix[2][3] = 0.0;

   matrix[3][3] = 1.0;
}

namespace
{

/// rotates vector by rotation part of matrix
void VectorRotate(const Vector3d& in1, const Matrix4d& in2, Vector3d& out)
{
   out = Vector3d(
      in1.Dot(in2.Column(0)),
      in1.Dot(in2.Column(1)),
      in1.Dot(in2.Column(2)));
}

/// rotates vector by the inverse of rotation matrix
void VectorIRotate(const Vector3d& in1, const Matrix4d& in2, Vector3d& out)
{
   //out.X(in1.X()*in2.Column(0).X() + in1.Y()*in2.Column(1).X() + in1.Z()*in2.Column(2).X());
   //out.Y(in1.X()*in2.Column(0).Y() + in1.Y()*in2.Column(1).Y() + in1.Z()*in2.Column(2).Y());
   //out.Z(in1.X()*in2.Column(0).Z() + in1.Y()*in2.Column(1).Z() + in1.Z()*in2.Column(2).Z());

   out = in2 * in1;

	//out[0] = in1[0]*in2[0][0] + in1[1]*in2[1][0] + in1[2]*in2[2][0];
	//out[1] = in1[0]*in2[0][1] + in1[1]*in2[1][1] + in1[2]*in2[2][1];
	//out[2] = in1[0]*in2[0][2] + in1[1]*in2[1][2] + in1[2]*in2[2][2];
}

/// transforms vector by matrix
void VectorTransform(const Vector3d& in1, const Matrix4d& in2, Vector3d& out)
{
   out = Vector3d(
      in1.Dot(in2.Column(0)),
      in1.Dot(in2.Column(1)),
      in1.Dot(in2.Column(2))) + in2.Row(3);

	//out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	//out[1] = DotProduct(in1, in2[1]) +	in2[1][3];
	//out[2] = DotProduct(in1, in2[2]) +	in2[2][3];
}

/// transforms vector by inverse of matrix
void VectorITransform(const Vector3d& in1, const Matrix4d& in2, Vector3d& out)
{
   Vector3d vTemp = in1 - in2.Row(3);
   VectorIRotate(vTemp, in2, out);

	//tmp[0] = in1[0] - in2[0][3];
	//tmp[1] = in1[1] - in2[1][3];
	//tmp[2] = in1[2] - in2[2][3];
	//VectorIRotate(tmp, in2, out);
}

} // unnamed namespace

using namespace MilkShape3D;

int AnimatedModel3d::FindMountJointIndex(LPCTSTR pszName) const
{
   return FindJointByName(pszName);
}

void AnimatedModel3d::Prepare()
{
   SetupJoints();
   SetupTangents();

   PrepareIndexBuffer();
}

void AnimatedModel3d::Upload()
{
   m_ibo.Update();
}

void AnimatedModel3d::SetupTangents()
{
   for (size_t i=0, iMax = GetData().m_vecJoints.size(); i<iMax; i++)
   {
      Joint& joint = GetData().m_vecJoints[i];

      const std::vector<KeyframeTrans>& vecPositionKeys = joint.PositionKeys();

      size_t numPositionKeys = vecPositionKeys.size();

      std::vector<Tangent>& vecTangents = joint.Tangents();
      vecTangents.resize(numPositionKeys);

      // if there are more than 2 keys, we can calculate tangents, otherwise we use zero derivatives
      if (numPositionKeys <= 2)
         continue;

      for (size_t k = 0; k < numPositionKeys; k++)
      {
         // make the curve tangents looped
         size_t k0 = k >= 1 ? k - 1 : numPositionKeys - 1 ;
         size_t k1 = k;
         size_t k2 = k + 1;
         if (k2 >= numPositionKeys)
            k2 = 0;

         // calculate the tangent, which is the vector from key[k - 1] to key[k + 1]
         Vector3d vTangent = vecPositionKeys[k2].Key() - vecPositionKeys[k0].Key();

         // weight the incoming and outgoing tangent by their time to avoid changes in speed, if the keys are not within the same interval
         double dt1 = vecPositionKeys[k1].Time() - vecPositionKeys[k0].Time();
         double dt2 = vecPositionKeys[k2].Time() - vecPositionKeys[k1].Time();
         double dt = dt1 + dt2;

         Vector3d vTangentIn = vTangent * (dt1 /dt);
         Vector3d vTangentOut = vTangent * (dt2 /dt);

         vecTangents[k1] = Tangent(vTangentIn, vTangentOut);
      }
   }
}

void AnimatedModel3d::PrepareIndexBuffer()
{
   const Data& data = GetData();

   m_vecGroupRenderData.resize(data.m_vecGroups.size());

   std::vector<unsigned int> vecVertexIndices;

   // prepare indices for each group
   for (size_t iGroup=0, iGroupMax=data.m_vecGroups.size(); iGroup<iGroupMax; iGroup++)
   {
      const Group& group = data.m_vecGroups[iGroup];

      // generate a list of vertex indices, building triangles of this group
      vecVertexIndices.resize(group.m_vecTriangleIndices.size() * 3);

      size_t uiTriangle = 0;
      for (size_t uiTriangleIndex : group.m_vecTriangleIndices)
      {
         unsigned int uiVertexIndex = uiTriangleIndex*3;

         for (unsigned int uiVertex = 0; uiVertex < 3; uiVertex++)
            vecVertexIndices[uiTriangle*3 + uiVertex] = uiVertexIndex + uiVertex;

         uiTriangle++;
      }

      GroupRenderData& renderData = m_vecGroupRenderData[iGroup];
      renderData.m_range = m_ibo.Add(vecVertexIndices);
   }
}

/// less function for classes that have a Time() function
template <typename T>
bool LessTime(const T& lhs, const T& rhs)
{
   return lhs.Time() < rhs.Time();
}

/// finds a frame time in a given std::vector that contains elements with a Time() function
template <typename T>
bool FindTime(const T& vList, double dFrame, size_t& uiPos1, size_t& uiPos2)
{
   // assumes that vList is ordered by Time()
   std::function<bool(const T::value_type&, const T::value_type&)> fnPred;
   fnPred = std::bind(&LessTime<T::value_type>, std::placeholders::_1, std::placeholders::_2);

   T::value_type t(dFrame);
   T::const_iterator iter = std::lower_bound(vList.begin(), vList.end(), t, fnPred);

   if (iter != vList.end())
   {
      if (iter == vList.begin())
         uiPos1 = 0;
      else
         uiPos1 = std::distance(vList.begin(), iter)-1;

      uiPos2 = uiPos1+1;
      return true;
   }

   return false;
}

void AnimatedModel3d::EvaluateJoint(const Joint& joint, JointRenderData& renderData,
                                    const std::vector<JointRenderData>& vecRenderData, double dFrame) const
{
   // calculate joint animation matrix, this matrix will animate matLocalSkeleton
   Vector3d vPos;
   EvaluatePosition(joint, dFrame, vPos);

   Quaternion4d quat(Vector3d(), 1.0);
   EvaluateRotation(joint, dFrame, quat);

   // make a matrix from pos/quat
   Matrix4d matAnimate = Matrix4d::Rotate(quat);
   matAnimate.Row(3, vPos);

   // animate the local joint matrix using: matLocal = matLocalSkeleton * matAnimate
   Matrix4d::Mult(renderData.LocalMatrix(), joint.LocalSkeletonMatrix(), matAnimate);

   // build up the hierarchy if joints
   if (joint.ParentIndex() == -1)
   {
      renderData.GlobalMatrix() = renderData.LocalMatrix();
   }
   else
   {
      // matGlobal = matGlobal(parent) * matLocal
      const JointRenderData& parentJoint = vecRenderData[joint.ParentIndex()];
      Matrix4d::Mult(renderData.GlobalMatrix(), parentJoint.GlobalMatrix(), renderData.LocalMatrix());
   }
}

void AnimatedModel3d::EvaluatePosition(const Joint& joint, double dFrame, Vector3d& vPos) const
{
   const std::vector<KeyframeTrans>& vecPositionKeys = joint.PositionKeys();

   if (vecPositionKeys.empty())
      return;

   // find the two keys, where "frame" is in between for the position keys
   size_t uiPos1, uiPos2;

   if (vecPositionKeys.size() == 1)
   {
      // there is only one key
      vPos = vecPositionKeys[0].Key();
   }
   else
   if (!FindTime(vecPositionKeys, dFrame, uiPos1, uiPos2))
   {
      // there are no such keys
      if (dFrame < vecPositionKeys[0].Time())
         vPos = vecPositionKeys[0].Key(); // take the first key
      else
      {
         // take the last key
         size_t uiUpper = vecPositionKeys.size() - 1;
         if (dFrame >= vecPositionKeys[uiUpper].Time())
            vPos = vecPositionKeys[uiUpper].Key();
      }
   }
   else
   {
      // there are such keys, so interpolate using hermite interpolation
      const KeyframeTrans& p0 = vecPositionKeys[uiPos1];
      const KeyframeTrans& p1 = vecPositionKeys[uiPos2];
      const Tangent& m0 = joint.Tangents()[uiPos1];
      const Tangent& m1 = joint.Tangents()[uiPos2];

      // normalize the time between the keys into [0..1]
      double t = (dFrame - p0.Time()) / (p1.Time() - p0.Time());
      double t2 = t * t;
      double t3 = t2 * t;

      // calculate hermite basis
      double h1 =  2.0 * t3 - 3.0 * t2 + 1.0;
      double h2 = -2.0 * t3 + 3.0 * t2;
      double h3 =        t3 - 2.0 * t2 + t;
      double h4 =        t3 -       t2;

      // do hermite interpolation
      vPos = h1 * p0.Key() + h3 * m0.TangentOut() + h2 * p1.Key() + h4 * m1.TangentIn();
   }
}

void AnimatedModel3d::EvaluateRotation(const Joint& joint, double dFrame, Quaternion4d& quat) const
{
   const std::vector<KeyframeRot>& vecRotationKeys = joint.RotationKeys();

   if (vecRotationKeys.empty())
      return;

   // find the two keys, where "frame" is in between for the rotation keys
   size_t uiPos1, uiPos2;

   if (vecRotationKeys.size() == 1)
   {
      // there is only one key
      quat.FromAngles(vecRotationKeys[0].Key().Data());
   }
   else
   if (!FindTime(vecRotationKeys, dFrame, uiPos1, uiPos2))
   {
      // if there are no such keys
      if (dFrame < vecRotationKeys[0].Time())
      {
         // take the first key
         quat.FromAngles(vecRotationKeys[0].Key().Data());
      }
      else
      {
         // take the last key
         size_t uiUpper = vecRotationKeys.size() - 1;
         if (dFrame >= vecRotationKeys[uiUpper].Time())
            quat.FromAngles(vecRotationKeys[uiUpper].Key().Data());
      }
   }
   else
   {
      // there are such keys, so do the quaternion slerp interpolation
      const KeyframeRot& r0 = vecRotationKeys[uiPos1];
      const KeyframeRot& r1 = vecRotationKeys[uiPos2];

      double t = (dFrame - r0.Time()) / (r1.Time() - r0.Time());

      Quaternion4d q1;
      q1.FromAngles(r0.Key().Data());

      Quaternion4d q2;
      q2.FromAngles(r1.Key().Data());

      quat = Quaternion4d::Slerp(q1, q2, t);
   }
}

void AnimatedModel3d::TransformVertex(const Vertex& vertex,
                                      const std::vector<JointRenderData>& vecRenderData,
                                      Vector3d& vVertex) const
{
   if (vertex.m_iJointIndex < 0)
   {
      vVertex = vertex.m_vPos;
      return;
   }

   Vector3d vTemp;

   const Joint& joint = GetData().m_vecJoints[vertex.m_iJointIndex];
   const JointRenderData& jointRenderData = vecRenderData[vertex.m_iJointIndex];

   VectorITransform(vertex.m_vPos, joint.GlobalSkeletonMatrix(), vTemp);
   VectorTransform(vTemp, jointRenderData.GlobalMatrix(), vVertex);
}

void AnimatedModel3d::TransformNormal(const Vertex& vertex,
                                      const std::vector<JointRenderData>& vecRenderData,
                                      Vector3d& vNormal) const
{
   if (vertex.m_iJointIndex < 0)
      return;

   unsigned int uiJointIndex = static_cast<size_t>(vertex.m_iJointIndex);

   ATLASSERT(uiJointIndex < GetData().m_vecJoints.size());
   const Joint& joint = GetData().m_vecJoints[uiJointIndex];

   ATLASSERT(uiJointIndex < vecRenderData.size());
   const JointRenderData& jointRenderData = vecRenderData[uiJointIndex];

   Vector3d vTemp;
   VectorIRotate(vNormal, joint.GlobalSkeletonMatrix(), vTemp);
   VectorRotate(vTemp, jointRenderData.GlobalMatrix(), vNormal);
}
