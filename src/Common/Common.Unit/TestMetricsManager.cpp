//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestMetricsManager.cpp Unit tests for class MetricsManager
//

// includes
#include "stdafx.h"
#include "MetricsManager.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class MetricsManager
TEST_CLASS(TestMetricsManager)
{
   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      MetricValue mv; mv;
      MetricTimeline mt; mt;
      MetricsManager mm; mm;
   }

   /// tests configuring metric as current value
   TEST_METHOD(TestMetricCurrentValue)
   {
      MetricsManager mm;
      mm.Config(Metrics::DataBytesTx, storeTypeCurrentValue);

      mm.Set(Metrics::DataBytesTx, 10);
      mm.Set(Metrics::DataBytesTx, 21);

      Assert::AreEqual<unsigned int>(21, mm.Get(Metrics::DataBytesTx), _T("last stored value of 21 must be returned"));
   }

   /// tests configuring metric as accumulated
   TEST_METHOD(TestMetricAccumulated)
   {
      MetricsManager mm;
      mm.Config(Metrics::DataBytesTx, storeTypeAccumulated);

      mm.Set(Metrics::DataBytesTx, 10);
      mm.Set(Metrics::DataBytesTx, 21);

      Assert::AreEqual<unsigned int>(31, mm.Get(Metrics::DataBytesTx), _T("accumulated value must be 31"));
   }

   /// tests using metric without configuring before
   TEST_METHOD(TestMetricWithoutConfig)
   {
      MetricsManager mm;

      mm.Set(Metrics::DataBytesTx, 10);
      mm.Set(Metrics::DataBytesTx, 21);

      Assert::AreEqual<unsigned int>(21, mm.Get(Metrics::DataBytesTx), _T("last value must be 21, since default is 'current value'"));
   }
};

} // namespace UnitTest
