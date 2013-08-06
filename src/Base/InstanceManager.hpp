//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file InstanceManager.hpp Instance manager for access to instances by interface type
//
#pragma once

// includes
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include <boost/ref.hpp>
#include <ulib/Exception.hpp>

/// \brief instance manager
/// manages object instances; uses Inversion of Control pattern
class InstanceManager: public boost::noncopyable
{
public:
   /// returns registered interface instance
   /// \tparam TInterface type of interface instance to retrieve
   template <typename TInterface>
   TInterface& Get()
   {
      CString cszName = SInstanceNameFromInterface<TInterface>()();
      T_mapAllInstances::iterator iter = m_mapAllInstances.find(cszName);
      if (iter == m_mapAllInstances.end())
         throw Exception(_T("Interface not registered: ") + cszName, __FILE__, __LINE__);

      boost::reference_wrapper<TInterface> ref =
         boost::any_cast<boost::reference_wrapper<TInterface> >(iter->second);

      return ref.get();
   }

   /// returns if an interface instance is available
   /// \tparam TInterface type of interface to check
   template <typename TInterface>
   bool IsAvail() const throw()
   {
      CString cszName = SInstanceNameFromInterface<TInterface>()();
      T_mapAllInstances::iterator iter = m_mapAllInstances.find(cszName);
      return iter != m_mapAllInstances.end();
   }

   /// registers an interface instance
   /// \tparam TInterface type of interface to register
   /// \remarks only one instance can be stored for every given interface type
   template <typename TInterface>
   void Register(boost::reference_wrapper<TInterface> ref)
   {
      CString cszName = SInstanceNameFromInterface<TInterface>()();
      m_mapAllInstances.insert(std::make_pair(cszName, ref));
   }

private:
   /// \brief template class to get instance name by interface type
   /// \tparam TInterface type of interface
   template <typename TInterface>
   struct SInstanceNameFromInterface
   {
      /// returns interface class name
      LPCTSTR operator()() const throw()
      {
         return TInterface::GetClassName();
      }
   };

   /// map type
   typedef std::map<CString, boost::any> T_mapAllInstances;

   /// instance map
   T_mapAllInstances m_mapAllInstances;
};
