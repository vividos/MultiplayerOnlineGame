//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Matrix4.hpp Matrix class (4x4)
//
#pragma once

// includes
#include "Vector3.hpp"
#include "Quaternion4.hpp"

/// \brief 4x4 matrix
/// \details values are ordered column-first: [1][2] is column 1, row 2.
template <typename T>
class Matrix4T
{
public:
   /// default ctor
   Matrix4T() throw()
   {
      std::uninitialized_fill(&m_d[0][0], &m_d[4][0], 0.0);
   }

   /// returns identity matrix
   static Matrix4T Identity() throw()
   {
      Matrix4T m;
      m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0;
      return m;
   }

   /// sets column with vector components
   void Column(unsigned int uiColumn, const Vector3T<T>& vColumn) throw()
   {
      ATLASSERT(uiColumn < 4);
      m_d[uiColumn][0] = vColumn.X();
      m_d[uiColumn][1] = vColumn.Y();
      m_d[uiColumn][2] = vColumn.Z();
   }

   /// returns column vector from components
   Vector3T<T> Column(unsigned int uiColumn) const throw()
   {
      ATLASSERT(uiColumn < 4);
      return Vector3T<T>(
         m_d[uiColumn][0],
         m_d[uiColumn][1],
         m_d[uiColumn][2]
      );
   }

   /// returns row vector from components
   Vector3T<T> Row(unsigned int uiRow) const throw()
   {
      ATLASSERT(uiRow < 4);
      return Vector3T<T>(
         m_d[0][uiRow],
         m_d[1][uiRow],
         m_d[2][uiRow]
      );
   }

   /// sets row with vector components
   void Row(unsigned int uiRow, const Vector3T<T>& vRow) throw()
   {
      ATLASSERT(uiRow < 4);
      m_d[0][uiRow] = vRow.X();
      m_d[1][uiRow] = vRow.Y();
      m_d[2][uiRow] = vRow.Z();
   }

   /// transpose matrix
   void Transpose() throw()
   {
      std::swap(m_d[0][1], m_d[1][0]);
      std::swap(m_d[0][2], m_d[2][0]);
      std::swap(m_d[0][3], m_d[3][0]);

      std::swap(m_d[1][2], m_d[2][1]);
      std::swap(m_d[1][3], m_d[3][1]);

      std::swap(m_d[2][3], m_d[3][2]);
   }

   /// transpose only the "rotation" components of the matrix
   void TransposeRotation() throw()
   {
      std::swap(m_d[0][1], m_d[1][0]);
      std::swap(m_d[0][2], m_d[2][0]);

      std::swap(m_d[1][2], m_d[2][1]);
   }

   /// multiply operator
   Vector3T<T> operator*(const Vector3T<T>& v) const throw()
   {
      return Vector3T<T>(
         m_d[0][0] * v.X() + m_d[1][0] * v.Y() + m_d[2][0] * v.Z() + m_d[3][0],
         m_d[0][1] * v.X() + m_d[1][1] * v.Y() + m_d[2][1] * v.Z() + m_d[3][1],
         m_d[0][2] * v.X() + m_d[1][2] * v.Y() + m_d[2][2] * v.Z() + m_d[3][2]
      );
   }

   /// multiply two matrices
   static void Mult(Matrix4T& matOut, const Matrix4T& matLeft, const Matrix4T& matRight) throw()
   {
      matOut[0][0] = matLeft[0][0] * matRight[0][0] + matLeft[0][1] * matRight[1][0] + matLeft[0][2] * matRight[2][0] + matLeft[0][3] * matRight[3][0];
      matOut[0][1] = matLeft[0][0] * matRight[0][1] + matLeft[0][1] * matRight[1][1] + matLeft[0][2] * matRight[2][1] + matLeft[0][3] * matRight[3][1];
      matOut[0][2] = matLeft[0][0] * matRight[0][2] + matLeft[0][1] * matRight[1][2] + matLeft[0][2] * matRight[2][2] + matLeft[0][3] * matRight[3][2];
      matOut[0][3] = matLeft[0][0] * matRight[0][3] + matLeft[0][1] * matRight[1][3] + matLeft[0][2] * matRight[2][3] + matLeft[0][3] * matRight[3][3];
      matOut[1][0] = matLeft[1][0] * matRight[0][0] + matLeft[1][1] * matRight[1][0] + matLeft[1][2] * matRight[2][0] + matLeft[1][3] * matRight[3][0];
      matOut[1][1] = matLeft[1][0] * matRight[0][1] + matLeft[1][1] * matRight[1][1] + matLeft[1][2] * matRight[2][1] + matLeft[1][3] * matRight[3][1];
      matOut[1][2] = matLeft[1][0] * matRight[0][2] + matLeft[1][1] * matRight[1][2] + matLeft[1][2] * matRight[2][2] + matLeft[1][3] * matRight[3][2];
      matOut[1][3] = matLeft[1][0] * matRight[0][3] + matLeft[1][1] * matRight[1][3] + matLeft[1][2] * matRight[2][3] + matLeft[1][3] * matRight[3][3];
      matOut[2][0] = matLeft[2][0] * matRight[0][0] + matLeft[2][1] * matRight[1][0] + matLeft[2][2] * matRight[2][0] + matLeft[2][3] * matRight[3][0];
      matOut[2][1] = matLeft[2][0] * matRight[0][1] + matLeft[2][1] * matRight[1][1] + matLeft[2][2] * matRight[2][1] + matLeft[2][3] * matRight[3][1];
      matOut[2][2] = matLeft[2][0] * matRight[0][2] + matLeft[2][1] * matRight[1][2] + matLeft[2][2] * matRight[2][2] + matLeft[2][3] * matRight[3][2];
      matOut[2][3] = matLeft[2][0] * matRight[0][3] + matLeft[2][1] * matRight[1][3] + matLeft[2][2] * matRight[2][3] + matLeft[2][3] * matRight[3][3];
      matOut[3][0] = matLeft[3][0] * matRight[0][0] + matLeft[3][1] * matRight[1][0] + matLeft[3][2] * matRight[2][0] + matLeft[3][3] * matRight[3][0];
      matOut[3][1] = matLeft[3][0] * matRight[0][1] + matLeft[3][1] * matRight[1][1] + matLeft[3][2] * matRight[2][1] + matLeft[3][3] * matRight[3][1];
      matOut[3][2] = matLeft[3][0] * matRight[0][2] + matLeft[3][1] * matRight[1][2] + matLeft[3][2] * matRight[2][2] + matLeft[3][3] * matRight[3][2];
      matOut[3][3] = matLeft[3][0] * matRight[0][3] + matLeft[3][1] * matRight[1][3] + matLeft[3][2] * matRight[2][3] + matLeft[3][3] * matRight[3][3];
   }

   /// builds a rotation matrix from axis and angle
   static Matrix4T<T> Rotate(Vector3T<T> vAxis, T dAngleRot)
   {
      vAxis.Normalize();

      T cs = std::cos(-dAngleRot);
      T sn = std::sin(-dAngleRot);
      T t = T(1.0) - cs;

      Matrix4T<T> matRotate;
      matRotate.Row(0, Vector3T<T>(t * vAxis.X() * vAxis.X() + cs, t * vAxis.X() * vAxis.Y() - sn * vAxis.Z(), t * vAxis.X() * vAxis.Z() + sn * vAxis.Y()));
      matRotate.Row(1, Vector3T<T>(t * vAxis.X() * vAxis.Y() + sn * vAxis.Z(), t * vAxis.Y() * vAxis.Y() + cs, t * vAxis.Y() * vAxis.Z() - sn * vAxis.X()));
      matRotate.Row(2, Vector3T<T>(t * vAxis.X() * vAxis.Z() - sn * vAxis.Y(), t * vAxis.Y() * vAxis.Z() + sn * vAxis.X(), t * vAxis.Z() * vAxis.Z() + cs));
      matRotate[3][3] = 1.0;
      return matRotate;
   }

   /// builds a rotation matrix from quaternion
   static Matrix4T<T> Rotate(Quaternion4d& quat)
   {
      Vector3T<T> vAxis;
      T dAngle;

      quat.ToAxisAngle(vAxis, dAngle);
      return Rotate(vAxis, dAngle);
   }

   /// access to const data
   const T* const Data() const throw() { return &m_d[0][0]; }

   /// access to non-const data
   T* const Data() throw() { return &m_d[0][0]; }

   /// proxy class for column access, write; used in operator[]
   class MatrixColumnProxyWrite
   {
   public:
      /// ctor
      MatrixColumnProxyWrite(Matrix4T& matrix, unsigned int uiColumn)
         :m_matrix(matrix),
          m_uiColumn(uiColumn)
      {
         ATLASSERT(uiColumn < 4);
      }

      /// write array operator access
      T& operator[](unsigned int uiRow) throw()
      {
         ATLASSERT(uiRow < 4);
         return m_matrix.m_d[m_uiColumn][uiRow];
      }

   private:
      /// matrix
      Matrix4T& m_matrix;
      /// column
      unsigned int m_uiColumn;
   };

   /// proxy class for column access, read-only; used in operator[]
   class MatrixColumnProxyRead
   {
   public:
      /// ctor
      MatrixColumnProxyRead(const Matrix4T& matrix, unsigned int uiColumn)
         :m_matrix(matrix),
          m_uiColumn(uiColumn)
      {
         ATLASSERT(uiColumn < 4);
      }

      /// read-only array operator access
      T operator[](unsigned int uiRow) const throw()
      {
         ATLASSERT(uiRow < 4);
         return m_matrix.m_d[m_uiColumn][uiRow];
      }

   private:
      /// matrix
      const Matrix4T& m_matrix;
      /// column
      unsigned int m_uiColumn;
   };

   /// array access operator; write
   MatrixColumnProxyWrite operator[](unsigned int uiColumn) throw()
   {
      ATLASSERT(uiColumn < 4);
      return MatrixColumnProxyWrite(*this, uiColumn);
   }

   /// array access operator; read-only
   MatrixColumnProxyRead operator[](unsigned int uiColumn) const throw()
   {
      ATLASSERT(uiColumn < 4);
      return MatrixColumnProxyRead(*this, uiColumn);
   }

private:
   friend MatrixColumnProxyRead;
   friend MatrixColumnProxyWrite;

   /// matrix values; column-first
   T m_d[4][4];
};

typedef Matrix4T<double> Matrix4d;
typedef Matrix4T<float> Matrix4f;
