//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AsyncActionQueue.hpp Asynchronous action queue
//
#pragma once

// includes
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "IActionQueue.hpp"
#include "Action.hpp"

// forward references
class IModel;

/// \brief maintains a queue of actions to be carried out asynchronously
/// each queue has its own strand to ensure that actions are carried out in
/// order of arrival
class AsyncActionQueue: public IActionQueue
{
public:
   /// ctor
   AsyncActionQueue(boost::asio::io_service& ioService, IModel& model)
      :m_ioService(ioService),
       m_model(model)
   {
   }

   /// posts action to be carried out immediately
   void Post(ActionPtr spAction)
   {
      m_ioService.dispatch(boost::bind(&AsyncActionQueue::Execute, this, spAction));
   }

private:
   /// executes action
   void Execute(ActionPtr spAction)
   {
      ObjectRef& arg = spAction->ArgumentRef();
      ATLASSERT(arg.m_sp != NULL);

      {
         // lock argument ref
         Lockable::LockType lock = arg.m_sp->Lock();
         spAction->Do(m_model);
      }
   }

private:
   /// io service
   boost::asio::io_service& m_ioService;

   /// model
   IModel& m_model;
};
