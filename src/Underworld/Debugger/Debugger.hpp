//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Debugger.hpp Debugger class
//
#pragma once

// forward references
class DebugClient;

/// debugger application
class Debugger
{
public:
   /// ctor
   Debugger(DebugClient& client);
   /// dtor
   ~Debugger();

   /// runs debugger
   int Run();

private:
   /// debug client
   DebugClient& m_client;
};
