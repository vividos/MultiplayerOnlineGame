//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IMessageSink.hpp Message sink interface
//
#pragma once

/// forward references
class RawMessage;

/// message sink interface
class IMessageSink
{
public:
   /// dtor
   virtual ~IMessageSink() {}

   /// called when new message has arrived; returns true when message has been handled
   virtual bool OnReceiveMessage(RawMessage& msg) = 0;
};
