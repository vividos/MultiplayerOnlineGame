//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MultiplayerOnlineGame.hpp doxygen documentation
//
#pragma once

/*! \mainpage

   \section Introduction

   MultiplayerOnlineGame is a prototype implementation of a game framework
   that can be used to implement many different games.

   \section Projects

   Here's a graphical representation of relations between all projects:
   \dot
   digraph projectrelations {
      node [shape=box]
      Base [ label="class Base" URL="\ref Base"];
      Client [ label="class Client" URL="\ref Client"];
      ClientLogic [ label="class ClientLogic" URL="\ref ClientLogic"];
      Common [ label="class Common" URL="\ref Common"];
      ConsoleServer [ label="class ConsoleServer" URL="\ref ConsoleServer"];
      Database [ label="class Database" URL="\ref Database"];
      ModelViewer [ label="class ModelViewer" URL="\ref ModelViewer"];
      RenderEngine [ label="class RenderEngine" URL="\ref RenderEngine"];
      Server [ label="class Server" URL="\ref Server"];
      ServerLogic [ label="class ServerLogic" URL="\ref ServerLogic"];
      TerrainViewer [ label="class TerrainViewer" URL="\ref TerrainViewer"];
      UserInterface [ label="class UserInterface" URL="\ref UserInterface"];

      Client -> ClientLogic
      ClientLogic -> Common
      Common -> Base
      ConsoleServer -> ServerLogic
      Database -> Common
      ModelViewer -> ClientLogic
      ModelViewer -> RenderEngine
      ModelViewer -> UserInterface
      RenderEngine -> Common
      Server -> ServerLogic
      ServerLogic -> Common
      ServerLogic -> Database
      TerrainViewer -> ClientLogic
      TerrainViewer -> RenderEngine
      TerrainViewer -> UserInterface
      UserInterface -> ClientLogic
      UserInterface -> RenderEngine
   }
   \enddot

   \section Base

   The Base project contains all classes and functions that are independent of
   any type of application to write.

   \section Common

   The Common project contains classes that are common to applications that
   use the client-server model.

*/
