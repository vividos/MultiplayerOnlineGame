//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ScatteredPointInterpolator.cpp Scattered point interpolator
//

// includes
#include "StdAfx.h"
#include "ScatteredPointInterpolator.hpp"
#define USE_EIGEN
#ifdef USE_EIGEN
#pragma warning(disable: 4714) // function 'T' marked as __forceinline not inlined
#pragma warning(disable: 4307) // '*' : integral constant overflow
#pragma warning(disable: 4308) // negative integral constant converted to unsigned type
#include <Eigen/Dense>
#else
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp> // for InvertMatrix()
#endif

namespace
{

double DistanceXY(const Vector3d& lhs, const Vector3d& rhs)
{
   return (Vector2d(lhs.X(), lhs.Y()) - Vector2d(rhs.X(), rhs.Y())).Length();
}

/// calculates distance of given vectors
template <typename TVector>
double Distance(const TVector& first, const TVector& second)
{
   return (first - second).Length();
}

#ifndef USE_EIGEN
/// Matrix inversion routine. Uses lu_factorize and lu_substitute in uBLAS to invert a matrix
/// \see https://savingyoutime.wordpress.com/2009/09/21/c-matrix-inversion-boostublas/
template<class T>
bool InvertMatrix(const boost::numeric::ublas::matrix<T>& input,
                  boost::numeric::ublas::matrix<T>& inverse)
{
   using namespace boost::numeric::ublas;

   typedef permutation_matrix<std::size_t> pmatrix;

   // create a working copy of the input
   matrix<T> A(input);

   // create a permutation matrix for the LU-factorization
   pmatrix pm(A.size1());

   // perform LU-factorization
   int res = lu_factorize(A, pm);
   if (res != 0)
      return false;

   // create identity matrix of "inverse"
   inverse.assign(identity_matrix<T> (A.size1()));

   // backsubstitute to get the inverse
   lu_substitute(A, pm, inverse);

   return true;
}
#endif

} // anonymous namespace

/// implemented as described in:
/// http://www.digitalrune.com/Support/Blog/tabid/719/EntryId/101/Scattered-Interpolation-using-Radial-Basis-Functions.aspx
ScatteredPointInterpolator::ScatteredPointInterpolator(double dSigma,
                                                       const std::vector<Vector3d>& vecPoints)
:m_dSigma(dSigma),
 m_vecPoints(vecPoints)
{
}

void ScatteredPointInterpolator::Init()
{
   // n: dimension of output vector // 1; the height value
   const size_t n = 1;

   // k: number of points
   const size_t k = m_vecPoints.size();

#ifdef USE_EIGEN
   // Eigen::Matrix() is row first, column second
   // calc G
   Eigen::MatrixXd G(k, k);

   for (size_t x=0; x<k; x++)
      for (size_t y=0; y<k; y++)
         G(x, y) = Gaussian(DistanceXY(m_vecPoints[x], m_vecPoints[y]));

   // calc Y
   Eigen::MatrixXd Y(n, k);
   for (size_t i=0; i<k; i++)
      Y(0, i) = m_vecPoints[i].Z();

   // calc w: solve Y=GW
   // invert G
   G.colPivHouseholderQr().solve(Y);
   Eigen::MatrixXd Ginv(k, k);
   Ginv = G;

   // W = Y * Ginv
   Eigen::MatrixXd W(k, n);
   W = Y * Ginv;

   m_vecWeights.resize(W.cols());

   for (size_t i=0, iMax=m_vecWeights.size(); i<iMax; i++)
      m_vecWeights[i] = W(0, i);

#else
   // calc G
   boost::numeric::ublas::matrix<double> G(k, k);

   for (size_t x=0; x<k; x++)
      for (size_t y=0; y<k; y++)
         G(x, y) = Gaussian(DistanceXY(m_vecPoints[x], m_vecPoints[y]));

   // calc Y
   boost::numeric::ublas::matrix<double> Y(n, k);
   for (size_t i=0; i<k; i++)
      Y(0, i) = m_vecPoints[i].Z();

   // calc w: solve Y=GW
   // invert G
   boost::numeric::ublas::matrix<double> Ginv(k, k);
   try
   {
      bool bRet = InvertMatrix(G, Ginv);
      if (!bRet)
         throw Exception(_T("invalid input data"), __FILE__, __LINE__);
   }
   catch(std::exception&)
   {
      throw Exception(_T("invalid input data"), __FILE__, __LINE__);
   }

   // W = Y * Ginv
   boost::numeric::ublas::matrix<double> W = boost::numeric::ublas::prod(Y, Ginv);

   // TODO replace with ublas function
   m_vecWeights.resize(W.size2());

   for (size_t i=0, iMax=m_vecWeights.size(); i<iMax; i++)
      m_vecWeights[i] = W(0, i);
#endif
}

double ScatteredPointInterpolator::Height(const Vector2d& vPos)
{
   // m: dimension of input vector // 2: the x and y values
//   size_t m = 2;

   size_t k = m_vecPoints.size();

   double dHeight = 0.0;
   for (size_t i=0; i<k; i++)
   {
      Vector2d xi(m_vecPoints[i].X(), m_vecPoints[i].Y());
      double dDist = Distance(vPos, xi);
      dHeight += m_vecWeights[i] * Gaussian(dDist);
   }

   return dHeight;
}

double ScatteredPointInterpolator::Gaussian(double x)
{
   // gaussian function
   return exp(-x*x/(2.0*m_dSigma*m_dSigma));
}
