//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MetricsManager.cpp Metrics manager
//

// includes
#include "StdAfx.h"
#include "MetricsManager.hpp"

void MetricsManager::Config(LPCTSTR pszMetricName, T_enMetricStoreType enStoreType)
{
   if (m_mapStoredValues.find(pszMetricName) == m_mapStoredValues.end())
   {
      m_mapStoredValues[pszMetricName] = StoredData();
   }
   else
   {
      // reset value
      m_mapStoredValues[pszMetricName].m_metricValue;
   }

   StoredData& data = m_mapStoredValues[pszMetricName];

   if (enStoreType != storeTypeTimeline && data.spTimeline != NULL)
      data.spTimeline.reset();

   data.enStoreType = enStoreType;

   if (enStoreType == storeTypeTimeline)
      data.spTimeline.reset(new MetricTimeline);
}

void MetricsManager::Set(LPCTSTR pszMetricName, unsigned int uiNewValue)
{
   if (m_mapStoredValues.find(pszMetricName) == m_mapStoredValues.end())
   {
      m_mapStoredValues[pszMetricName] = StoredData();
   }

   StoredData& data = m_mapStoredValues[pszMetricName];

   switch (data.enStoreType)
   {
   case storeTypeCurrentValue:
      m_mapStoredValues[pszMetricName].m_metricValue = MetricValue(uiNewValue);
      break;

   case storeTypeAccumulated:
      m_mapStoredValues[pszMetricName].m_metricValue.Add(uiNewValue);
      break;

   case storeTypeTimeline:
      // TODO store in timeline
      // also store as current value
      m_mapStoredValues[pszMetricName].m_metricValue = MetricValue(uiNewValue);
      break;
   }
}

unsigned int MetricsManager::Get(LPCTSTR pszMetricName)
{
   if (m_mapStoredValues.find(pszMetricName) == m_mapStoredValues.end())
   {
      m_mapStoredValues[pszMetricName] = StoredData();
   }

   return m_mapStoredValues[pszMetricName].m_metricValue.Value();
}
