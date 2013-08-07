//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file LogCategories.hpp Logging categories
//
#pragma once

/// \brief logging categories
namespace Log
{
   static TCHAR General[] = _T("general");      ///< general messages
   static TCHAR Service[] = _T("service");      ///< windows service messages
   static TCHAR Threading[] = _T("threading");  ///< threading messages
   static TCHAR Session[] = _T("session");      ///< user session messages

   /// server specific log categories
   namespace Server
   {
      static TCHAR General[] =   _T("server.general");   ///< general server messages
      static TCHAR Auth[] =      _T("server.auth");      ///< authentication messages
      static TCHAR AuthSRP[] =   _T("server.auth.srp");  ///< SRP auth messages
      static TCHAR Session[] =   _T("server.session");   ///< session messages
      static TCHAR Database[] =  _T("server.database");  ///< database messages
      static TCHAR Service[] =   _T("server.service");   ///< service messages
      static TCHAR TextMessages[] = _T("server.textmessages"); ///< text messages

      namespace World
      {
         static TCHAR Player[] = _T("server.world.player"); ///< Player messages
         static TCHAR Region[] = _T("server.world.region"); ///< region messages
         static TCHAR Mobile[] = _T("server.world.mobile"); ///< Mobile messages
      }
   }

   /// client specific log categories
   namespace Client
   {
      static TCHAR General[] =   _T("client.general");      ///< general client messages
      static TCHAR Auth[] =      _T("client.auth");         ///< authentication messages
      static TCHAR AuthSRP[] =   _T("client.auth.srp");     ///< SRP auth messages
      static TCHAR Model[] =     _T("client.model");        ///< client's model messages
      static TCHAR Controller[] =_T("client.controller");   ///< controller messages
      static TCHAR Window[] =    _T("client.window");       ///< window manager messages
      static TCHAR Renderer[] =  _T("client.renderer");     ///< render engine messages
      static TCHAR OpenGL[] =    _T("client.opengl");       ///< OpenGL-specific messages
      static TCHAR Audio[] =     _T("client.audio");        ///< Audio subsystem messages
      static TCHAR AudioMusicDirector[] = _T("client.audio.musicdirector"); ///< MusicDirector messages
   }
}
