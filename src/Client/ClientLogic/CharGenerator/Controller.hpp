//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file CharGenerator/Controller.hpp Character generator controller
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "IViewController.hpp"

// forward references
class KeyboardActionManager;
class PerspectiveCamera;

namespace CharGenerator
{
   /// controller for character generation view
   class CLIENTLOGIC_DECLSPEC Controller : public IViewController
   {
   public:
      /// ctor
      Controller(KeyboardActionManager& keyboardActionManager,
         std::shared_ptr<PerspectiveCamera> spCamera);
      /// dtor
      virtual ~Controller();

      // virtual methods from IViewController

      /// called on mouse button event
      virtual void OnMouseButtonEvent(bool /*bPressed*/, T_enMouseButtonType /*enButtonType*/, int /*x*/, int /*y*/) override {}

      /// called on mouse motion event
      virtual void OnMouseMotionEvent(int /*x*/, int /*y*/, int /*xrel*/, int /*yrel*/) override {}

      /// tick processing
      virtual void Tick() {}

   private:
      /// camera
      std::shared_ptr<PerspectiveCamera> m_spCamera;
   };

} // namespace CharGenerator
