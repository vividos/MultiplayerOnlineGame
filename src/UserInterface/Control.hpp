//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file Control.hpp Control base class
//
#pragma once

// includes
#include "Window.hpp"
#include <ulib/Exception.hpp>

/// control base class
class Control: public Window
{
public:
   /// dtor
   virtual ~Control() throw() {}

   /// registers child; always throws exception: Control cannot be used as parent class for child windows
   virtual void RegisterChild(WindowPtr) override
   {
      throw Exception(_T("cannot use Control as parent"), __FILE__, __LINE__);
   }

   /// unregisters child; always throws exception: Control cannot be used as parent class for child windows
   virtual void UnregisterChild(WindowPtr) override
   {
      throw Exception(_T("cannot use Control as parent"), __FILE__, __LINE__);
   }

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(Window)
   END_ATTR_MAP()
};
