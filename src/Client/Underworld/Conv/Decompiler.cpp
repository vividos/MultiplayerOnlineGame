//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Conv/Decompiler.cpp Underworld conversation code decompiler
//

// includes
#include "stdafx.h"
#include "Decompiler.hpp"
#include "CodeGraph.hpp"
#include "ConvLoader.hpp"
#include "GameStrings.hpp"

using namespace Conv;

Decompiler::Decompiler(Uint16 uiConv, IFileSystem& fileSystem, GameStrings& strings)
:m_uiConv(uiConv),
m_strings(strings)
{
   Conv::CodeVM codeVM;

   Import::ConvLoader::LoadConvCode(fileSystem, codeVM, uiConv);

   std::vector<std::string> vecStrings = strings.GetStringBlock(0x0e00 + uiConv);

   m_spCodeGraph.reset(new Conv::CodeGraph(codeVM.CodeSegment(),
      0,
      static_cast<Uint16>(codeVM.CodeSegment().size()),
      vecStrings,
      codeVM.ImportedFuncs(),
      codeVM.ImportedGlobals(),
      codeVM.GlobalsReserved()));

   m_spCodeGraph->Decompile();
}

std::string Decompiler::GetName() const
{
   return m_strings.Get(7, m_uiConv + 16);
}

void Decompiler::Write(FILE* fd)
{
   fputs("; conversation for ", fd);
   fputs(m_strings.Get(7, m_uiConv + 16).c_str(), fd);
   fputs("\n\n", fd);
   fflush(fd);

   int indentLevel = 0;
   const Conv::CodeGraph::T_GraphList& g = m_spCodeGraph->GetGraph();
   Conv::CodeGraph::T_ConstGraphIterator iter = g.begin(), stop = g.end();
   for (; iter != stop; ++iter)
   {
#if 0
      if (iter->m_isProcessed && iter->m_labelName.length() != 0 && iter->m_xrefCount > 0)
      {
         fputs(iter->m_labelName.c_str(), fd);
         fputs(":\n", fd);
      }
#else
      if (iter->m_isProcessed)
         continue;
#endif

      if (iter->m_type == Conv::typeStatement && iter->statement_data.indent_change_before != 0)
         indentLevel += iter->statement_data.indent_change_before;
      UaAssert(indentLevel >= 0);

      if (iter->m_type == Conv::typeFuncStart)
      {
         std::string funcName = iter->m_labelName;

         if (funcName.find("unused_") == 0 || iter->m_xrefCount == 0)
         {
            while (iter != stop && iter->m_type != Conv::typeFuncEnd)
               ++iter;

            // jump over opcodes from function end
            std::advance(iter, 3);

            continue;
         }
      }

      //      if (!iter->m_isProcessed && iter->m_labelName.length() != 0)
      if (iter->m_labelName.length() != 0)
      {
         fputs(iter->m_labelName.c_str(), fd);
         fputs(":\n", fd);
      }

      for (int i = 0; i<indentLevel; i++)
         fputs("   ", fd);

      std::string text = iter->Format();
      fputs(text.c_str(), fd);

#if 0
      if (iter->m_type == Conv::typeFuncStart)
      {
         std::string funcName = iter->m_labelName;

         const Conv::CodeGraph::T_mapFunctions& functionMap = graph.GetFunctionMap();

         UaAssert(functionMap.find(funcName) != functionMap.end());

         const Conv::FuncInfo& funcInfo = functionMap.find(funcName)->second;

         if (!funcInfo.caller.empty())
         {
            fputs(", caller xrefs: ", fd);
            std::for_each(funcInfo.caller.begin(), funcInfo.caller.end(), [fd](const std::string& caller)
            {
               fputs(caller.c_str(), fd);
               fputs(", ", fd);
            });
         }
      }
#endif

      fputs("\n", fd);
      fflush(fd);

      if (iter->m_type == Conv::typeStatement && iter->statement_data.indent_change_after != 0)
         indentLevel += iter->statement_data.indent_change_after;
      UaAssert(indentLevel >= 0);
   }
}
