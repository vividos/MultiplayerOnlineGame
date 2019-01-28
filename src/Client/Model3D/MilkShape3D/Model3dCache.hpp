//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\Model3dCache.hpp MilkShape3D model cache
//
#pragma once

// includes
#include <ulib/thread/LightweightMutex.hpp>
#include <map>

namespace MilkShape3D
{
   /// cache for *Model3d objects
   template <typename TModelType>
   class Model3dCache
   {
   public:
      /// ctor
      Model3dCache() throw() {}

      /// checks if given named model is in cache
      bool IsAvail(const CString& cszName) const throw()
      {
         LightweightMutex::LockType lock(m_mtxMap);

         return m_mapNameToData.find(cszName) != m_mapNameToData.end();
      }

      /// returns named model; must be in cache
      std::shared_ptr<TModelType> Get(const CString& cszName) const throw()
      {
         ATLASSERT(IsAvail(cszName));

         LightweightMutex::LockType lock(m_mtxMap);

         return m_mapNameToData.find(cszName)->second;
      }

      /// adds (or replaces) model
      void Add(const CString& cszName, std::shared_ptr<TModelType> spData) throw()
      {
         LightweightMutex::LockType lock(m_mtxMap);

         m_mapNameToData[cszName] = spData;
      }

   private:
      /// mutex to protect access to map
      mutable LightweightMutex m_mtxMap;

      /// mapping from name to model
      std::map<CString, std::shared_ptr<TModelType>> m_mapNameToData;
   };

} // namespace MilkShape3D
