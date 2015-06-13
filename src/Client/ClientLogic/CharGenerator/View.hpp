//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file CharGenerator/View.hpp Character generator View class
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "IView.hpp"

namespace CharGenerator
{
   /// view for character generator
   class CLIENTLOGIC_DECLSPEC View : public IView
   {
   public:
      View();
      virtual ~View() throw() {}
   };

} // namespace CharGenerator
