//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\AnimationData.hpp Animation data for Warriors & Commoners model pack
//
#pragma once

// includes
#include "Model3DCommon.hpp"
#include "IModelDisplayState.hpp"

namespace MilkShape3D
{
/// \brief data for the Frogames CS Warriors & Commoners model pack
/// \see http://www.frogames.net/content-packs/cs-warriors-and-commoners.html
namespace WarriorsAndCommoners
{

/// animation data for Warriors & Commoners model pack
class MODEL3D_DECLSPEC AnimationData : public boost::noncopyable
{
public:
   /// returns all available animation names
   static void EnumAnimations(std::vector<CString>& vecAnimations);

   /// returns animation frames by animation name
   static void GetAnimationFrames(const CString& cszAnimation, unsigned int& uiStartFrame, unsigned int& uiEndFrame);

   /// returns animation frames by animation enum and weapon type enum
   static void GetAnimationFrames(T_enModelAnimation enAnimation,
      T_enModelAnimationWeaponType enAnimWeaponType,
      unsigned int& uiStartFrame, unsigned int& uiEndFrame);

private:
   /// ctor
   AnimationData();
   /// dtor
   ~AnimationData();
};

} // namespace WarriorsAndCommoners
} // namespace MilkShape3D
