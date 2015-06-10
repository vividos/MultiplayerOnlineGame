//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Conv/Conversation.cpp Conversation
//
#pragma once

// includes
#include "stdafx.h"
#include "Conversation.hpp"
#include "Underworld.hpp"
#include "StringTools.hpp"

using namespace Conv;
using namespace Underworld;

// Conversation methods

void Conversation::Init(unsigned int uiConvLevel,
   Uint16 uiConvObjPos,
   CodeCallback* pCodeCallback,
   std::vector<std::string>& vecLocalStrings)
{
   m_uiConvLevel = uiConvLevel;
   m_uiConvObjPos = uiConvObjPos,

   m_vecLocalStrings.insert(m_vecLocalStrings.begin(), vecLocalStrings.begin(), vecLocalStrings.end());

   CodeVM::Init(pCodeCallback, m_uw.GetPlayer().GetConvGlobals());
}

void Conversation::Done()
{
   // write back conv. globals
   CodeVM::Done(m_uw.GetPlayer().GetConvGlobals());
}

std::string Conversation::GetLocalString(Uint16 uiStringNr)
{
   // TODO really subtract 1?
   UaAssert(static_cast<size_t>(uiStringNr - 1) < m_vecLocalStrings.size());
   return m_vecLocalStrings[uiStringNr - 1];
}

Uint16 Conversation::AllocString(const char* pszStr)
{
   std::string str(pszStr);
   size_t pos = m_vecLocalStrings.size();
   m_vecLocalStrings.push_back(str);
   return static_cast<Uint16>(pos);
}

/// The following functions are implemented:
/// * = implemented, x = assert
///
///   babl_menu
///   babl_fmenu
///   print
///   babl_ask
/// * compare
/// * random
/// x plural
/// * contains
/// x append
/// x copy
/// x find
/// * length
/// x val
/// x say
/// x respond
/// * get_quest
/// * set_quest
/// * sex
///   show_inv
///   give_to_npc
///   give_ptr_npc
///   take_from_npc
///   take_id_from_npc
///   identify_inv
///   do_offer
///   do_demand
///   do_inv_create
///   do_inv_delete
///   check_inv_quality
///   set_inv_quality
///   count_inv
///   setup_to_barter
///   end_barter
///   do_judgement
///   do_decline
///   pause
///   set_likes_dislikes
///   gronk_door
///   set_race_attitude
///   place_object
///   take_fronpc_inv
///   add_to_npc_inv
///   remove_talker
///   set_attitude
///   x_skills
///   x_traps
///   x_obj_pos
///   x_obj_stuff
///   find_inv
///   find_barter
///   find_barter_total
void Conversation::ImportedFunc(const char* pszFuncName)
{
   std::string funcname(pszFuncName);

   Uint16 argpos = m_stack.StackPtr();
   Uint16 argcount = m_stack.At(argpos);
   argpos--;

   if (funcname.compare("compare")==0)
   {
      UaAssert(argcount == 2);

      // get arguments
      Uint16 arg1 = m_stack.At(argpos--);
      arg1 = m_stack.At(arg1);

      Uint16 arg2 = m_stack.At(argpos);
      arg2 = m_stack.At(arg2);

      // get strings
      std::string str1(m_vecLocalStrings[arg1]), str2(m_vecLocalStrings[arg2]);

      StringTools::ConvertToLowercase(str1);
      StringTools::ConvertToLowercase(str2);

      // check if first string contains second
      m_uiResultRegister = str1 == str2;
   }
   else
   if (funcname.compare("random")==0)
   {
      UaAssert(argcount == 1);

      Uint16 arg = m_stack.At(argpos--);
      arg = m_stack.At(arg);

      // this code assumes that rand() can return RAND_MAX

      // rnum is in the range [0..1[
      double rnum = double(rand())/double(RAND_MAX+1);
      rnum *= arg; // now in range [0..arg[
      m_uiResultRegister = Uint16(rnum + 1.0); // now from [1..arg+1[
   }
   else
   if (funcname.compare("plural")==0)
   {
      UaTrace("Conversation: intrinsic plural() not implemented");
      UaAssert(false);
   }
   else
   if (funcname.compare("contains")==0)
   {
      UaAssert(argcount == 2);

      // get arguments
      Uint16 arg1 = m_stack.At(argpos--);
      arg1 = m_stack.At(arg1);

      Uint16 arg2 = m_stack.At(argpos);
      arg2 = m_stack.At(arg2);

      // get strings
      std::string str1(m_vecLocalStrings[arg1]), str2(m_vecLocalStrings[arg2]);

      StringTools::ConvertToLowercase(str1);
      StringTools::ConvertToLowercase(str2);

      // check if first string contains second
      m_uiResultRegister = str1.find(str2) != std::string::npos;
   }
   else
   if (funcname.compare("append")==0)
   {
      UaTrace("Conversation: intrinsic append() not implemented");
      UaAssert(false);
   }
   else
   if (funcname.compare("copy")==0)
   {
      UaTrace("Conversation: intrinsic copy() not implemented");
      UaAssert(false);
   }
   else
   if (funcname.compare("find")==0)
   {
      UaTrace("Conversation: intrinsic find() not implemented");
      UaAssert(false);
   }
   else
   if (funcname.compare("length")==0)
   {
      UaAssert(argcount == 1);

      // get argument
      Uint16 arg = m_stack.At(argpos--);
      arg = m_stack.At(arg);

      // return string length
      m_uiResultRegister = static_cast<Uint16>(m_vecLocalStrings[arg].size());
   }
   else
   if (funcname.compare("val")==0)
   {
      UaTrace("Conversation: intrinsic val() not implemented");
      UaAssert(false);
   }
   else
   if (funcname.compare("say")==0)
   {
      UaTrace("Conversation: intrinsic say() not implemented");
      UaAssert(false);
   }
   else
   if (funcname.compare("respond")==0)
   {
      UaTrace("Conversation: intrinsic respond() not implemented");
      UaAssert(false);
   }
   else
   if (funcname.compare("sex")==0)
   {
      UaAssert(argcount == 2);

      Uint16 arg1 = m_stack.At(argpos--);
      arg1 = m_stack.At(arg1);

      Uint16 arg2 = m_stack.At(argpos);
      arg2 = m_stack.At(arg2);

      // check player gender
      if (m_uw.GetPlayer().GetAttribute(attrGender) == 0)
         arg1 = arg2;

      m_uiResultRegister = arg1;
   }
   else
   {
      CodeVM::ImportedFunc(pszFuncName);
   }
}

Uint16 Conversation::GetGlobal(const char* pszGlobName)
{
   std::string globname(pszGlobName);
   Uint16 val = 0;

   // get npc object to talk to
   ObjectPtr obj = m_uw.GetLevelList().
      GetLevel(m_uiConvLevel).GetObjectList().GetObject(m_uiConvObjPos);

   NpcObject& npc = CastToNpcObject(obj);

   NpcInfo& npcInfo = npc.GetNpcInfo();

   if (globname.compare("play_name")==0)
      val = AllocString(CStringA(m_uw.GetPlayer().GetName()));
   else if (globname.compare("npc_xhome")==0)      val = npcInfo.npc_xhome;
   else if (globname.compare("npc_yhome")==0)      val = npcInfo.npc_yhome;
   else if (globname.compare("npc_attitude")==0)   val = npcInfo.npc_attitude;
   else if (globname.compare("npc_goal")==0)       val = npcInfo.npc_goal;
   else if (globname.compare("npc_gtarg")==0)      val = npcInfo.npc_gtarg;
   else if (globname.compare("npc_hp")==0)         val = npcInfo.npc_hp;
   else if (globname.compare("npc_hunger")==0)     val = npcInfo.npc_hunger;
   else if (globname.compare("npc_level")==0)      val = npcInfo.npc_level;
   else if (globname.compare("npc_talkedto")==0)   val = npcInfo.npc_talkedto ? 1 : 0;
   else if (globname.compare("npc_whoami")==0)     val = npcInfo.npc_whoami;
   else
   {
      return CodeVM::GetGlobal(pszGlobName);
   }

   UaTrace("Conversation::GetGlobal %hs returned %04x\n", pszGlobName, val);

   return val;
}

/// globals that still need implementation:
/// play_hunger
/// play_health
/// play_arms
/// play_power
/// play_hp
/// play_mana
/// play_level
/// new_player_exp
/// play_poison
/// play_drawn
/// play_sex
/// npc_health
/// npc_arms
/// npc_power
/// npc_name
/// dungeon_level
/// riddlecounter
/// game_time
/// game_days
/// game_mins
void Conversation::SetGlobal(const char* pszGlobName, Uint16 val)
{
   UaTrace("Conversation::SetGlobal: %hs = %04x\n", pszGlobName, val);

   std::string globname(pszGlobName);

   Player& pl = m_uw.GetPlayer();

   // get npc object to talk to
   ObjectPtr obj = m_uw.GetLevelList().
      GetLevel(m_uiConvLevel).GetObjectList().GetObject(m_uiConvObjPos);

   NpcObject& npc = CastToNpcObject(obj);

   NpcInfo& npcInfo = npc.GetNpcInfo();
   Uint8 val8 = static_cast<Uint8>(val & 0xff);

   if (globname.compare("play_name")==0)
   {
      UaAssert(val < m_vecLocalStrings.size());
      pl.Player::SetName(m_vecLocalStrings[val].c_str());
   }
   else if (globname.compare("npc_xhome")==0)      npcInfo.npc_xhome = val8;
   else if (globname.compare("npc_yhome")==0)      npcInfo.npc_yhome = val8;
   else if (globname.compare("npc_attitude")==0)   npcInfo.npc_attitude = val8;
   else if (globname.compare("npc_goal")==0)       npcInfo.npc_goal = val8;
   else if (globname.compare("npc_gtarg")==0)      npcInfo.npc_gtarg = val8;
   else if (globname.compare("npc_hp")==0)         npcInfo.npc_hp = val8;
   else if (globname.compare("npc_hunger")==0)     npcInfo.npc_hunger = val8;
   else if (globname.compare("npc_level")==0)      npcInfo.npc_level = val8;
   else if (globname.compare("npc_talkedto")==0)   npcInfo.npc_talkedto = val != 0;
   else if (globname.compare("npc_whoami")==0)     npcInfo.npc_whoami = val8;
   else
   {
      CodeVM::SetGlobal(pszGlobName, val);
   }
}
