//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Singleton.hpp Singleton template class
//
#pragma once

// includes
#pragma warning(push)
#pragma warning(disable: 4800) // 'T' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4100) // 't' : unreferenced formal parameter
#pragma warning(disable: 4244) // 'argument' : conversion from 'T1' to 'T2', possible loss of data
#include <boost/thread/mutex.hpp>
#pragma warning(pop)

/// \brief thread-safe Double Check Singleton Pattern implementation
/// \note use the IMPLEMENT_SINGLETON macro to implement singleton instance in a .cpp file
/// \see http://www.chaoticmind.net/~hcb/projects/boost.atomic/doc/atomic/usage_examples.html#boost_atomic.usage_examples.singleton
template <typename T>
class Singleton
{
public:
   /// ctor
   Singleton() throw()
   {
   }

   /// \brief returns instance of singleton
   /// ensures that singleton is only created once
   static T& Instance()
   {
      T* tmp = m_instance.load(std::memory_order_consume);
      if (!tmp)
      {
         boost::mutex::scoped_lock l(m_instantiationMutex);
         tmp = m_instance.load(boost::memory_order_consume);
         if (!tmp)
         {
            tmp = new T;
            m_instance.store(tmp, std::memory_order_release);
         }
      }
      return *tmp;
   }

private:
   /// instance of object
   static std::atomic<T*> m_instance;

   /// mutex to protect singleton creation
   static boost::mutex m_instantiationMutex;
};

/// use this macro to implement a singleton instance in a .cpp file
#define IMPLEMENT_SINGLETON(T) \
   std::atomic<T*> Singleton<T>::m_instance(0); \
   std::mutex Singleton<T>::m_instantiationMutex;
