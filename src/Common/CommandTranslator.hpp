//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file CommandTranslator.hpp Command to Action translator
//
#pragma once

// includes
#include "Common.hpp"

// forward references
class IModel;
class Command;

/// \brief translates commands to action(s)
class COMMON_DECLSPEC CommandTranslator
{
public:
   /// ctor
   CommandTranslator(IModel& model);
   /// dtor
   ~CommandTranslator() throw();

   /// translate command and forward to model
   void ReceiveCommand(Command& cmd);

   /// checks command for plausibility using the model
   bool CheckCommand(const Command& cmd);

private:
   /// model
   IModel& m_model;
};
