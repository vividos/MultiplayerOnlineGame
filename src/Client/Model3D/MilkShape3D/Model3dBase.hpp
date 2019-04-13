//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\Model3dBase.hpp MilkShape3D model base class
//
#pragma once

// includes
#include "IModel3d.hpp"
#include "MilkShape3D/Data.hpp"

namespace MilkShape3D
{

/// MilkShape3D model base class
class Model3dBase: public IModel3d
{
public:
   /// ctor
   Model3dBase() {}
   /// dtor
   virtual ~Model3dBase() {}

protected:
   friend class ModelManager;
   friend class ModelDisplayState;

   const Data& GetData() const { return m_data; } ///< returns model data; const version
         Data& GetData() { return m_data; } ///< returns model data

   /// finds joint index by joint name, or returns -1
   int FindJointByName(const CString& cszJointName) const;

   /// sets up joints
   void SetupJoints();

protected:
   /// model data
   Data m_data;
};

} // namespace MilkShape3D
