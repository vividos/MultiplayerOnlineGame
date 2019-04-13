//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IModel.hpp MVC model interface
//
#pragma once

// includes
#include <vector>
#include "Object.hpp"

// forward references
class Command;
class MovementInfo;
class TimeIndex;

/**
   \section Model View Controller class relations

   The Model View Controller pattern separates the data model from displaying
   it, and from actually modifying the model. Since in a multiplayer game
   there is only one (representative) model, but many views and controllers,
   an adapted MVC pattern is used. The pattern uses a (from client
   perspective) remote model or world model, and a local model for every
   client. Every client also has a view and controller. The controller has to
   send its commands to the remote model, where it is checked. The resulting
   action is then sent to all clients. The controller that sent an action can
   apply a command to its local model, but must be prepared that these
   actions can be canceled or denied by the server.

   The classes to implement the adapted MVC pattern are flexible enough to be
   switched together to also support local only games. Here the different
   scenarios are described:

   Local Model (e.g. single player game)
   \dot
   digraph localmodel {
      node [shape=box]
      View [ label="local view" URL="IView"];
      Model [ label="LocalModel" URL="LocalModel"];
      Controller [ label="LocalController" URL="LocalController"];

      View -> Model [label="shows"]
      Model -> View [label="notifies about updates"]
      Controller -> Model [label="send command"]
      Model -> Model [label="updates, actions"]
   }
   \enddot

   Distributed Model (e.g. multiplayer game)
   \dot
   digraph distributedmodel {
      node [shape=box, fontname=Courier]
      subgraph clusterLocal
      {
         style=filled;
         color=lightgrey;

         ClientView [ label="client view" URL="IView" ];
         ClientModel [ label="ClientModel" URL="ClientModel" ];
         ClientController [ label="ClientController" URL="ClientController" ];

         ClientView -> ClientModel [label="shows"]
         ClientModel -> ClientView [label="notifies about updates"]
         ClientController -> ClientModel [label="send hints"]
      }
      subgraph clusterRemote
      {
         style=filled;
         color=lightgrey;

         ServerModel [ label="ServerModel" URL="ServerModel" ];
         ServerController [ label="ServerController" URL="ServerController" ];

         ServerController -> ServerModel [label="check command"]
      }
      subgraph clusterWorld
      {
         style=filled;
         color=lightgrey;

         WorldModel [ label="WorldModel" URL="WorldModel" ];
      }

      ClientController -> ServerController [label="send command"]

      ServerModel -> ClientModel [label="updates, actions"]

      ServerController -> WorldModel [label="send command"]
      WorldModel -> ServerModel [label="updates, actions"]
   }
   \enddot
*/

/// \brief the "model" in the MVC pattern
/// \details duties of this class:
/// * converts commands to one or more actions
/// * applies actions to objects/mobiles
/// * forwards actions and updates to another model, if any (e.g. ClientModel)
/// * applies updates to objects
class IModel
{
public:
   DEFINE_INSTANCE(IModel)

   /// dtor
   virtual ~IModel() {}

   /// initializes model
   virtual void InitialUpdate(MobilePtr spPlayer) = 0;

   /// does model processing
   virtual void Tick(const TimeIndex& timeIndex) = 0;

   /// called to receive an action from another model (e.g. the server-side model)
   virtual void ReceiveAction(ActionPtr spAction) = 0;

   /// called to receive commands from a controller (local or received from client)
   virtual void ReceiveCommand(Command& c) = 0;

   /// called when objects are being added and/or removed
   virtual void AddRemoveObject(const std::vector<ObjectPtr>& vecObjectsToAdd,
      const std::vector<ObjectId>& vecObjectsToRemove) = 0;

   /// updates movement info for one object
   virtual void UpdateObjectMovement(const ObjectId& id, const MovementInfo& info) = 0;
};
