//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ISession.hpp Session interface
//
#pragma once

// forward references
class Message;

/// session interface
class ISession: public boost::noncopyable
{
public:
   /// dtor
   virtual ~ISession() throw() {}

   /// sends message to recipient
   virtual void SendMessage(const Message& msg) = 0;
};
