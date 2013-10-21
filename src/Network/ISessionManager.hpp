//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ISessionManager.hpp session manager interface
//
#pragma once

// forward references
class Session;
class ServerSession;
class Uuid;
typedef Uuid ObjectId;

/// session manager interface
class ISessionManager
{
public:
   /// dtor
   virtual ~ISessionManager() throw() {}

   /// called to create new session
   virtual std::shared_ptr<Session> CreateNewSession() = 0;

   /// connects a session to an object id
   virtual void ConnectSession(const ObjectId& objId, std::shared_ptr<Session> spSession) = 0;

   /// returns session by object id
   virtual std::weak_ptr<Session> FindSession(const ObjectId& objId) = 0;
};
