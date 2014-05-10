//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Conv/CodeVM.cpp Conversation code execution functions
//
#pragma once

// includes
#include "stdafx.h"
#include "CodeVM.hpp"
#include "Opcodes.hpp"
#include "ConvGlobals.hpp"
#include <sstream>

using Conv::CodeCallback;
using Conv::CodeVM;

// CodeVM methods

CodeVM::CodeVM()
:m_pCodeCallback(NULL)
{
   m_uiInstrPtr = m_uiBasePtr = m_uiResultRegister = 0xffff;
   m_uiCallLevel = m_uiGlobalsReserved = 0;
   m_bFinished = true;
}

void CodeVM::Init(CodeCallback* pCodeCallback, const Underworld::ConvGlobals& cg)
{
   m_pCodeCallback = pCodeCallback;

   // reset pointer
   m_uiInstrPtr = 0;
   m_uiBasePtr = 0xffff;
   m_uiResultRegister = 0;
   m_bFinished = false;
   m_uiCallLevel = 1;

   // init stack: 4k should be enough for anybody.
   m_stack.Init(4096);

   // reserve stack for globals/private globals
   m_stack.StackPtr(m_uiGlobalsReserved);

   // load private globals onto stack
   {
      const std::vector<Uint16>& glob = cg.GetVectorGlobals(m_uiConvSlot);

      Uint16 max = static_cast<Uint16>(glob.size());
      for (Uint16 i=0; i<max; i++)
         m_stack.Set(i, glob[i]);
   }

   // load imported globals onto stack
   {
      std::map<Uint16,ImportedItem>::iterator iter, stop;
      iter = m_mapImportedGlobals.begin();
      stop = m_mapImportedGlobals.end();

      for (;iter!=stop; iter++)
      {
         Uint16 pos = (*iter).first;
         ImportedItem& iitem = (*iter).second;

         Uint16 val = GetGlobal(iitem.m_strName.c_str());

         m_stack.Set(pos,val);
      }
   }
}

void CodeVM::Done(Underworld::ConvGlobals& cg)
{
   if (m_uiConvSlot == 0xffff)
      return;

   // store back globals from stack
   std::vector<Uint16>& vecGlobals = cg.GetVectorGlobals(m_uiConvSlot);

   Uint16 max = static_cast<Uint16>(vecGlobals.size());

   for (Uint16 i=0; i<max; i++)
      vecGlobals[i] = m_stack.At(i);
}

bool CodeVM::Step()
{
   if (m_bFinished)
      return false;

   UaAssert(m_uiInstrPtr < m_code.size());

   Uint16 arg1 = 0, arg2 = 0;
   Uint16 opcode = m_code[m_uiInstrPtr];

   // execute one instruction
   switch (opcode)
   {
   case op_NOP:
      break;

   case op_OPADD:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg1 + arg2);
      break;

   case op_OPMUL:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg1 * arg2);
      break;

   case op_OPSUB:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 - arg1);
      break;

   case op_OPDIV:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      if (arg1==0)
      {
         UaTrace("CodeVM: OPDIV: division by zero\n");
         m_bFinished = true;
         return false;
      }
      m_stack.Push(arg2 / arg1);
      break;

   case op_OPMOD:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      if (arg1==0)
      {
         UaTrace("CodeVM: OPMOD: division by zero\n");
         m_bFinished = true;
         return false;
      }
      m_stack.Push(arg2 % arg1);
      break;

   case op_OPOR:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 || arg1);
      break;

   case op_OPAND:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 && arg1);
      break;

   case op_OPNOT:
      m_stack.Push(!m_stack.Pop());
      break;

   case op_TSTGT:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 > arg1);
      break;

   case op_TSTGE:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 >= arg1);
      break;

   case op_TSTLT:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 < arg1);
      break;

   case op_TSTLE:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 <= arg1);
      break;

   case op_TSTEQ:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 == arg1);
      break;

   case op_TSTNE:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg2 != arg1);
      break;

   case op_JMP:
      m_uiInstrPtr = m_code[m_uiInstrPtr+1]-1;
      break;

   case op_BEQ:
      arg1 = m_stack.Pop();
      if (arg1 == 0)
         m_uiInstrPtr += m_code[m_uiInstrPtr+1];
      else
         m_uiInstrPtr++;
      break;

   case op_BNE:
      arg1 = m_stack.Pop();
      if (arg1 != 0)
         m_uiInstrPtr += m_code[m_uiInstrPtr+1];
      else
         m_uiInstrPtr++;
      break;

   case op_BRA:
      m_uiInstrPtr += m_code[m_uiInstrPtr+1];
      break;

   case op_CALL: // local function
      // stack value points to next instruction after call
      m_stack.Push(m_uiInstrPtr+1);
      m_uiInstrPtr = m_code[m_uiInstrPtr+1]-1;
      m_uiCallLevel++;
      break;

   case op_CALLI: // imported function
      {
         arg1 = m_code[++m_uiInstrPtr];

         std::string funcname;
         if (m_mapImportedFuncs.find(arg1) == m_mapImportedFuncs.end())
         {
            UaTrace("CodeVM: couldn't find imported function 0x%04x\n", arg1);
            m_bFinished = true;
            return false;
         }

         ImportedFunc(m_mapImportedFuncs[arg1].m_strName.c_str());
      }
      break;

   case op_RET:
      if (--m_uiCallLevel)
      {
         // conversation ended
         m_bFinished = true;
      }
      else
      {
         arg1 = m_stack.Pop();
         m_uiInstrPtr = arg1;
      }
      break;

   case op_PUSHI:
      m_stack.Push(m_code[++m_uiInstrPtr]);
         break;

   case op_PUSHI_EFF:
      m_stack.Push(m_uiBasePtr + (Sint16)m_code[++m_uiInstrPtr]);
      break;

   case op_POP:
      m_stack.Pop();
      break;

   case op_SWAP:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      m_stack.Push(arg1);
      m_stack.Push(arg2);
      break;

   case op_PUSHBP:
      m_stack.Push(m_uiBasePtr);
      break;

   case op_POPBP:
      arg1 = m_stack.Pop();
      m_uiBasePtr = arg1;
      break;

   case op_SPTOBP:
      m_uiBasePtr = m_stack.StackPtr();
      break;

   case op_BPTOSP:
      m_stack.StackPtr(m_uiBasePtr);
      break;

   case op_ADDSP:
      {
         arg1 = m_stack.Pop();

         // fill reserved stack space with dummy values
         for(int i=0; i<arg1; i++)
            m_stack.Push(0xdddd);
      }
      break;

   case op_FETCHM:
      arg1 = m_stack.Pop();

      FetchValue(arg1);

      m_stack.Push(m_stack.At(arg1));
      break;

   case op_STO:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();

      StoreValue(arg2,arg1);

      m_stack.Set(arg2,arg1);
      break;

   case op_OFFSET:
      arg1 = m_stack.Pop();
      arg2 = m_stack.Pop();
      arg1 += arg2 - 1 ;
      m_stack.Push(arg1);
      break;

   case op_START:
      // do nothing
      break;

   case op_SAVE_REG:
      arg1 = m_stack.Pop();
      m_uiResultRegister = arg1;
      break;

   case op_PUSH_REG:
      m_stack.Push(m_uiResultRegister);
      break;

   case op_EXIT_OP:
      // finish processing (we still might be in some sub function)
      m_bFinished = true;
      break;

   case op_SAY_OP:
      arg1 = m_stack.Pop();
      SayOp(arg1);
      break;

   case op_RESPOND_OP:
      // do nothing
      break;

   case op_OPNEG:
      arg1 = m_stack.Pop();
      m_stack.Push(-arg1);
      break;

   default: // unknown opcode
      UaTrace("CodeVM: unknown opcode 0x%04x\n", opcode);
      m_bFinished = true;
      break;
   }

   // process next instruction
   ++m_uiInstrPtr;

   return !m_bFinished;
}

void CodeVM::ReplacePlaceholder(std::string& str)
{
   std::string::size_type pos = 0;
   while ((pos = str.find('@',pos)) != std::string::npos)
   {
      char source = str[pos+1];
      char vartype = str[pos+2];

      signed int param = 0;
      unsigned int value = 0;
      unsigned int num_len = 0;
      {
         const char* startpos = str.c_str()+pos;
         char* endpos = NULL;
         param = (signed int)strtol(startpos+3,&endpos,10);
         num_len = endpos-startpos;
      }

      // get param value
      switch(source)
      {
      case 'G': // conv global, directly from stack
         value = m_stack.At(static_cast<Uint16>(static_cast<unsigned int>(param)));
         break;
      case 'S': // value from parameter passed to function
         value = m_stack.At(static_cast<Uint16>(static_cast<unsigned int>(m_uiBasePtr + param)));
         break;
      case 'P': // pointer value from parameter passed to function
         param = m_stack.At(static_cast<Uint16>(static_cast<unsigned int>(m_uiBasePtr + param)));
         value = m_stack.At(static_cast<Uint16>(static_cast<unsigned int>(param)));
         break;
      }

      std::string varstr;

      switch(vartype)
      {
      case 'S': // string
         varstr = GetLocalString(static_cast<Uint16>(value));
         break;
      case 'I': // integer
         {
            std::ostringstream buffer;
            buffer << value;
            varstr.assign(buffer.str());
         }
         break;
      }

      // insert value string
      str.replace(pos, num_len, varstr.c_str());
   }
}

void CodeVM::SetResultRegister(Uint16 val)
{
   m_uiResultRegister = val;
}

void CodeVM::ImportedFunc(const char* pszFuncName)
{
   UaTrace("CodeVM: executing function \"%hs\" with %u arguments\n",
      pszFuncName, m_stack.At(m_stack.StackPtr()));

   if (std::string(pszFuncName) == "babl_menu")
   {
      std::vector<Uint16> uiAnswerStringIds;

      //Uint16 arg1=m_stack.At(m_stack.StackPtr()); // ignored
      Uint16 arg2=m_stack.At(m_stack.StackPtr()-1);
      while (m_stack.At(arg2)!=0)
      {
         uiAnswerStringIds.push_back(m_stack.At(arg2));
         arg2++;
      }

      m_uiResultRegister = m_pCodeCallback->BablMenu(uiAnswerStringIds);
   }
   else
      m_uiResultRegister = m_pCodeCallback->ExternalFunc(pszFuncName, m_stack);
}

std::string CodeVM::GetLocalString(Uint16 /*uiStrNum*/)
{
   UaAssert(false); // implement in your derived class
   return std::string("");
}

void CodeVM::SayOp(Uint16 uiStringId)
{
   m_pCodeCallback->Say(uiStringId);
}

Uint16 CodeVM::GetGlobal(const char* pszGlobName)
{
   UaTrace("CodeVM::GetGlobal: unknown global %hs\n", pszGlobName);
   return 0;
}

void CodeVM::SetGlobal(const char* pszGlobName, Uint16 val)
{
   UaTrace("CodeVM::SetGlobal: unknown global %hs = %04x\n", pszGlobName, val);
}

void CodeVM::StoreValue(Uint16 at, Uint16 val)
{
   std::map<Uint16,ImportedItem>::iterator iter =
      m_mapImportedGlobals.find(at);

   if (iter != m_mapImportedGlobals.end())
      UaTrace("CodeVM::StoreValue: %hs = %04x\n", iter->second.m_strName.c_str(), val);
}

void CodeVM::FetchValue(Uint16 at)
{
   std::map<Uint16,ImportedItem>::iterator iter =
      m_mapImportedGlobals.find(at);

   if (iter != m_mapImportedGlobals.end())
      UaTrace("CodeVM::FetchValue: %hs returned %04x\n", iter->second.m_strName.c_str(), m_stack.At(at));
}
