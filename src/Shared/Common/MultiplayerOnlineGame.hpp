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

      subgraph clusterShared
      {
         label="Shared";
         style=filled;
         color=lightgrey;

         Base [ label="Base" URL="\ref Base"];
         Common [ label="Common" URL="\ref Common"];
         Network [ label="Network" URL="\ref Network"];
         SRP [ label="SRP" URL="\ref SRP"];
         Terrain [ label="Terrain" URL="\ref Terrain"];
      }

      subgraph clusterClient
      {
         label="Client";
         style=filled;
         color=lightgrey;

         Arena [ label="Arena" URL="\ref Arena"];
         Audio [ label="Audio" URL="\ref Audio"];
         AudioPlayer [ label="AudioPlayer" URL="\ref AudioPlayer"];
         ClientLogic [ label="ClientLogic" URL="\ref ClientLogic"];
         ConsoleClient [ label="ConsoleClient" URL="\ref ConsoleClient"];
         GameClient [ label="GameClient" URL="\ref GameClient"];
         Model3D [ label="Model3D" URL="\ref Model3D"];
         ModelViewer [ label="ModelViewer" URL="\ref ModelViewer"];
         RenderEngine [ label="RenderEngine" URL="\ref RenderEngine"];
         TerrainViewer [ label="TerrainViewer" URL="\ref TerrainViewer"];
         Underworld [ label="Underworld" URL="\ref Underworld"];
         UserInterface [ label="UserInterface" URL="\ref UserInterface"];
         WorldBuilder [ label="WorldBuilder" URL="\ref WorldBuilder"];
      }

      subgraph clusterServer
      {
         label="Server";
         style=filled;
         color=lightgrey;

         ConsoleServer [ label="ConsoleServer" URL="\ref ConsoleServer"];
         Database [ label="Database" URL="\ref Database"];
         GameServer [ label="GameServer" URL="\ref GameServer"];
         ServerLogic [ label="ServerLogic" URL="\ref ServerLogic"];
         World [ label="World" URL="\ref World"];
      }

      Common -> Base
      Network -> Common
      Network -> SRP
      Terrain -> Common
      Terrain -> RenderEngine

      Arena -> Audio
      Arena -> ClientLogic
      Arena -> Model3D
      Arena -> Network
      Arena -> RenderEngine
      Arena -> Terrain
      Arena -> UserInterface
      Arena -> World
      Audio -> Common
      AudioPlayer -> Audio
      ClientLogic -> Audio
      ClientLogic -> RenderEngine
      ClientLogic -> UserInterface
      ConsoleClient -> ClientLogic
      ConsoleClient -> Network
      GameClient -> Audio
      GameClient -> ClientLogic
      GameClient -> Model3D
      GameClient -> Network
      GameClient -> RenderEngine
      GameClient -> Terrain
      GameClient -> UserInterface
      Model3D -> RenderEngine
      ModelViewer -> Model3D
      ModelViewer -> ClientLogic
      ModelViewer -> RenderEngine
      ModelViewer -> UserInterface
      RenderEngine -> Common
      TerrainViewer -> ClientLogic
      TerrainViewer -> Model3D
      TerrainViewer -> RenderEngine
      TerrainViewer -> Terrain
      TerrainViewer -> UserInterface
      Underworld -> ClientLogic
      Underworld -> RenderEngine
      UserInterface -> RenderEngine
      WorldBuilder -> ClientLogic
      WorldBuilder -> RenderEngine

      ConsoleServer -> ServerLogic
      GameServer -> ServerLogic
      ServerLogic -> Database
      ServerLogic -> Network
   }
   \enddot

   It is implied that all projects reference project \ref Base, and some may
   reference project \ref Common. The lines were omitted in the diagram to
   simplify the diagram.

   \section Base

   The Base project contains all classes and functions that are independent of
   any type of application to write.

   \section Common

   The Common project contains classes that are common to applications that
   use the client-server model.

   \section Network

   The Network project contains all classes needed to communicate between server
   and client. See base class Message for all messages, interface IAuthManager
   for authentication manager, and interface IModel and the section
   \ref Model View Controller class relations how the classes relate to one
   another.

   \section SRP

   The SRP project contains an implementation of the SRP secure remote
   authentication protocol. See namespace SRP for more infos.

   \section Terrain

   The Terrain project contains data classes and a render manager class to
   support rendering terrain data. See class Terrain::View::TerrainRenderManager for
   more infos.

   \section Arena

   The Arena project contains a single-player game that uses all projects
   needed to build the game.

   \section Audio

   The Audio project contains the implementation of audio facilities for
   a game. See namespace Audio, interface Audio::IAudioManager and class
   Audio::AudioManager for more infos. The class Audio::MusicDirector
   implements an automatic way for the game to interact with background music
   played.

   \section AudioPlayer

   The AudioPlayer project contains an ATL project to test and showcase the
   classes in the \ref Audio project.

   \section ClientLogic

   The ClientLogic project contains classes to implement a game client, either
   communicating with a server or as a standalone game. See the class
   GameClientBase as starting place for own game clients, and the Scene class
   and ISceneManager as a way to display game scenes. The namespace CharGenerator
   contains character generator classes, and the namespace GameMenu contains a
   usable game menu implementation.

   \section ConsoleClient

   The ConsoleClient project contains a console application that serves as a test
   client to develop connecting to a server. See class ConsoleClient for more infos.

   \section GameClient

   The GameClient project contains an implementation of a game client that connects
   to a server, specifically the \ref GameServer.

   \section Model3D

   The Model3D project contains classes to load and render 3D models that act as
   static and animated objects in the game world. See generic interfaces IModel3d,
   IModelDisplayState as well as the class ModelRenderManager to be used to manage
   models being rendered. The project contains implementation to load MD2 models
   (from Quake 2), see MD2::ModelManager, and an implementation to load MilkShape3D
   models (format .ms3d) that have greater flexibility in ways to animate them.
   See namespace MilkShape3D and class MilkShape3D::ModelManager for more infos.

   \section ModelViewer

   The ModelViewer project contains a simple viewer for MilkShape3D models. It also
   allows to change animation being played back, and changing model attributes.

   \section RenderEngine

   The RenderEngine project contains classes to render using OpenGL. There are many
   unconnected helper classes usable to build higher-level renderers, such as
   ModelRenderManager or TerrainRenderManager.

   \section TerrainViewer

   The TerrainViewer project contains a simple viewer for terrain rendering. It
   uses the \ref Terrain project to render terrain data. Terrain data can be
   generated using the \ref WorldBuilder project.

   \section Underworld

   The Underworld project contains classes to implement a remake of the Ultima
   Underworld[TM] game. The classes are ported from the "Underworld Adventures"
   game (http://uwadv.sourceforge.net/). The project exists check if the other
   projects are of any use to implement an actual game.

   \section UserInterface

   The UserInterface project contains classes to render a 2D user interface
   using OpenGL, on top of the scene already displayed. See interface
   IWindowManager and its implementation WindowManager for more infos. All
   window elements share the base class Window.

   \section WorldBuilder

   The WorldBuilder project contains a WTL project with ribbon bar and OpenGL
   child view that implements a world generator. The project generates terrain
   data as well as static and dynamic objects. See class WorldRenderManager
   how the world is rendered, and the class WorldGenerator how the world is
   generated.

   \section ConsoleServer

   The ConsoleServer project contains a console project to run a game server
   on the console. It can be used for debugging purposes. The project
   instantiates an object of class GameServer.

   \section Database

   The Database project implements access to a game database that is stored
   on the server. See class Database::Manager for more infos.

   \section GameServer

   The GameServer project implements a Windows service that runs a game
   server. The project instantiates an object of class GameServer.

   \section ServerLogic

   The ServerLogic project contains classes to implement a game server,
   including communication, authentication and session management. It also
   runs an instance of the WorldModel. See class GameServer for more infos.

   \section World

   The World project contains classes to run an instance of a world. Most
   game object classes are in the \ref Common project, but those that are
   only needed on the server are placed here.

*/
