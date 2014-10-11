//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ActionMessage.hpp Action message
//
#pragma once

// includes
#include "Network.hpp"
#include "Message.hpp"
#include "Action.hpp"

// forward references
class ByteStream;

/// message that wraps an action
class NETWORK_DECLSPEC ActionMessage: public Message
{
public:
   /// ctor; when sending action, pass action here
   ActionMessage(ActionPtr spAction = ActionPtr())
      :Message(msgAction),
       m_spAction(spAction)
   {
   }
   /// dtor
   virtual ~ActionMessage() throw() {}

   /// returns action
   ActionPtr GetAction() throw() { return m_spAction; }

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

private:
   /// action
   ActionPtr m_spAction;
};
