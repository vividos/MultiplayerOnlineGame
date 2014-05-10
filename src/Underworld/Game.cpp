//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Game.cpp main function
//

// includes
#include "stdafx.h"
#include "Game.hpp"
#include "ConvLoader.hpp"
#include "StringsPakFile.hpp"
#include "PropertiesLoader.hpp"
#include "LevelImporter.hpp"
#include "Decompiler.hpp"

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

void Game::Decompile()
{
   for (Uint16 block = 0x0e00; block < 0x0fff; block++)
   {
      if (!m_strings.IsBlockAvail(block))
         continue;

      Uint16 uiConv = block - 0x0e00;

      Conv::Decompiler d(uiConv, m_fileSystem, m_strings);

      std::string name = d.GetName();

      std::ostringstream buffer;
      buffer << "d:\\Projekte\\decompile-" << uiConv;
      if (!name.empty())
         buffer << "-" << name;

      buffer << ".txt";

      FILE* fd = nullptr;
      errno_t err = fopen_s(&fd, buffer.str().c_str(), "wt");
      if (err != 0 || fd == nullptr)
         return;

      d.Write(fd);

      fclose(fd);
   }
}
