//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MetricsManager.hpp Metrics manager
//
#pragma once

// includes
#include "Common.hpp"
#include <map>

/// metric names
namespace Metrics
{
   static LPCTSTR DataBytesTx = _T("DataBytesTx"); ///< data bytes sent
   static LPCTSTR DataBytesRx = _T("DataBytesRx"); ///< data bytes received
   static LPCTSTR MessagesTx = _T("MessagesTx"); ///< number of messages sent
   static LPCTSTR MessagesRx = _T("MessagesRx"); ///< number of messages received

   /// client metric names
   namespace Client
   {
      static LPCTSTR FramesPerSecond = _T("FramesPerSecond"); ///< frames per second
      static LPCTSTR NumFrames = _T("NumFrames"); ///< number of frames rendered
      static LPCTSTR NumKeyPresses = _T("NumKeyPresses"); ///< number of key presses done
      static LPCTSTR NumMouseClicks = _T("NumMouseClicks"); ///< number of mouse clicks done

      static LPCTSTR TimeOnline = _T("TimeOnline"); ///< number of seconds online
      static LPCTSTR CurrentPing = _T("CurrentPing"); ///< current ping value, in milliseconds
   }

   /// server metric names
   namespace Server
   {
      static LPCTSTR Uptime = _T("Uptime"); ///< current uptime; number of seconds online
      static LPCTSTR TimeOnline = _T("TimeOnline"); ///< number of seconds online ever

      static LPCTSTR CurrentNumConnections = _T("CurrentNumConnections"); ///< current number of connections
      static LPCTSTR NumConnectionsAccepted = _T("NumConnectionsAccepted"); ///< number of connections accepted
      static LPCTSTR NumConnectionsRejected = _T("NumConnectionsRejected"); ///< number of connections rejected

      static LPCTSTR LogonQueueSize = _T("LogonQueueSize"); ///< size of logon queue
   }
}

/// metric value
class MetricValue
{
public:
   /// ctor
   MetricValue()
      :m_uiValue(0)
   {
   }

   /// returns value
   unsigned int Value() const { return m_uiValue; }

private:
   friend class MetricsManager;

   /// ctor to initialize with value
   MetricValue(unsigned int uiValue)
      :m_uiValue(uiValue)
   {
   }

   /// adds value; used for accumulated values
   void Add(unsigned int uiValue)
   {
      m_uiValue += uiValue;
   }

private:
   /// value
   unsigned int m_uiValue;
};

/// metric store type
enum T_enMetricStoreType
{
   storeTypeCurrentValue,  ///< only current value is stored
   storeTypeAccumulated,   ///< vales are stored in accumulator
   storeTypeTimeline,      ///< vales are stored as timeline
};

/// metric timeline
class MetricTimeline
{
public:
};

/// metrics manager
class COMMON_DECLSPEC MetricsManager
{
public:
   /// (re-)configures metric store type
   void Config(LPCTSTR pszMetricName, T_enMetricStoreType enStoreType);

   /// sets new value
   void Set(LPCTSTR pszMetricName, unsigned int uiNewValue);

   /// returns current value
   unsigned int Get(LPCTSTR pszMetricName);

private:
   /// stored data for metric
   struct StoredData
   {
      /// ctor
      StoredData()
         :enStoreType(storeTypeCurrentValue)
      {
      }

      /// store type
      T_enMetricStoreType enStoreType;

      /// value
      MetricValue m_metricValue;

      /// timeline; used when type storeTypeTimeline
      std::shared_ptr<MetricTimeline> spTimeline;
   };

   /// stored metrics values
   std::map<CString, StoredData> m_mapStoredValues;
};
