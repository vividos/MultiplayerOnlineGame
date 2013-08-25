//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file Observable.hpp Observable event class
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include <set>
#include <map>
#include <boost/function.hpp>

/// starts new event map; parent classes event map is ignored
#define BEGIN_EVENT_MAP() \
   virtual void GetEvents(std::set<CString>& setEvents) const throw() { setEvents;

/// extends event map from parent
#define INHERIT_EVENT_MAP(BaseClass) \
   BaseClass::GetEvents(setEvents);

/// event name entry
#define EVENT_ENTRY(EventName) \
   setEvents.insert(EventName);

/// event map end
#define END_EVENT_MAP() \
   }


/// \brief a class that can be observed
/// \details design is inspired by ExtJS event handling
class USERINTERFACE_DECLSPEC Observable
{
public:
   /// event id for registered event
   typedef unsigned int T_RegisteredEventId;

   /// event handler function
   typedef boost::function<void()> T_fnEventHandler;

   /// ctor
   Observable() throw()
      :m_bInited(false)
   {
   }

   /// returns events; implemented by BEGIN_EVENT_MAP macro
   virtual void GetEvents(std::set<CString>& /*setEvents*/) const throw() = 0
   {
      ATLASSERT(false); // must use BEGIN_EVENT_MAP macro to implement event map!
   }

   /// registers an event
   T_RegisteredEventId On(const CString& cszEventName, T_fnEventHandler fnEventHandler)
   {
      Init();
      ATLASSERT(IsAvailEvent(cszEventName));

      T_RegisteredEventId eventId = s_uiNextEventId++;

      if (m_mapAllHandler.find(cszEventName) == m_mapAllHandler.end())
         m_mapAllHandler.insert(std::make_pair(cszEventName, std::map<T_RegisteredEventId, T_fnEventHandler>()));

      m_mapAllHandler[cszEventName].insert(std::make_pair(eventId, fnEventHandler));

      return eventId;
   }

   /// unregisters an event by given event id
   bool Un(T_RegisteredEventId id)
   {
      Init();
      std::map<CString, std::map<T_RegisteredEventId, T_fnEventHandler> >::iterator
         iter = m_mapAllHandler.begin(),
         stop = m_mapAllHandler.end();

      for (; iter != stop; ++iter)
      {
         std::map<T_RegisteredEventId, T_fnEventHandler>& mapAllHandler = iter->second;

         std::map<T_RegisteredEventId, T_fnEventHandler>::iterator found = mapAllHandler.find(id);
         if (found != mapAllHandler.end())
         {
            mapAllHandler.erase(found);
            return true; // removed handler
         }
      }

      return false; // not found
   }

   /// returns if object supports event with given name
   bool IsAvailEvent(const CString& cszEventName) const throw()
   {
      Init();
      return m_setAllEvents.find(cszEventName) != m_setAllEvents.end();
   }

   /// fires event; all handler are notified
   void FireEvent(const CString& cszEventName) const
   {
      Init();
      ATLASSERT(IsAvailEvent(cszEventName));

      if (m_mapAllHandler.find(cszEventName) == m_mapAllHandler.end())
         return; // no handler registered yet

      // call all event handler
      const std::map<T_RegisteredEventId, T_fnEventHandler>& mapAllHandler = m_mapAllHandler.find(cszEventName)->second;

      std::map<T_RegisteredEventId, T_fnEventHandler>::const_iterator iter = mapAllHandler.begin(),
         stop = mapAllHandler.end();

      for (; iter != stop; ++iter)
         iter->second();
   }

private:
   /// initializes event system
   void Init() const throw()
   {
      if (m_bInited)
         return;

      GetEvents(const_cast<std::set<CString>&>(m_setAllEvents));

      const_cast<bool&>(m_bInited) = true;
   }

private:
   /// next event id
   static T_RegisteredEventId s_uiNextEventId;

   /// all available events
   std::set<CString> m_setAllEvents;

   /// mapping with event names to map with all handlers
   std::map<CString, std::map<T_RegisteredEventId, T_fnEventHandler> > m_mapAllHandler;

   /// indicates if event list is inited
   bool m_bInited;
};
