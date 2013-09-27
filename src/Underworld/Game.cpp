//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Game.cpp main function
//

// includes
#include "stdafx.h"
#include "Game.hpp"
#include "ConvLoader.hpp"
#include "StringsPakFile.hpp"
#include "PropertiesLoader.hpp"
#include "LevelImporter.hpp"

void Game::Init()
{
   // TODO replace with path from settings
   m_fileSystem.AddArchive(_T("D:\\Projekte\\UW_DEMO.ZIP"));

   {
      Import::StringsPakFile file(m_fileSystem.OpenFile(_T("data\\strings.pak"), true));
      file.LoadAllBlocks(m_strings);
   }

   {
      Import::PropertiesLoader propLoader(m_fileSystem);
      propLoader.Load(m_world.GetObjectProperties());
   }

   {
      Import::LevelImporter importer(m_fileSystem);
      importer.LoadUwDemoLevel(m_world.GetLevelList());
   }

   Import::ConvLoader::LoadConvGlobals(m_fileSystem, m_world.GetPlayer().GetConvGlobals(), true);
}

void Game::Done()
{
}
