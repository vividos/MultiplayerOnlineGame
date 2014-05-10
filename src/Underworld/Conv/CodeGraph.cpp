//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Conv/CodeGraph.cpp Conversation code graph for decompiler
//

// includes
#include "stdafx.h"
#include "CodeGraph.hpp"
#include "Opcodes.hpp"
#include <sstream>
#include <iomanip>

using namespace Conv;

//
// CodeGraphItem
//

std::string CodeGraphItem::Format() const
{
   std::string item_text;

   switch (m_type)
   {
   case typeFuncStart:
      item_text = "function start";
      break;

   case typeFuncEnd:
      item_text = "function end";
      break;

   case typeOpcode:
      item_text = FormatOpcode();
      break;

   case typeStatement:
      item_text = statement_data.statement;
      break;

   case typeOperator:
      {
         std::ostringstream buffer;
         buffer <<
            (m_isProcessed ? "   // " : "") <<
            "operator " <<
            g_aInstructions[operator_data.op_opcode].mnemonic <<
            ", needs " << operator_data.needed_expr << " expressions, yields " <<
            (operator_data.returns_expr ? "an" : "no") << " expression;" <<
            (operator_data.sto_swap_args ? " swaps args;" : "") <<
            " level=" << operator_data.prec_level;

         item_text = buffer.str();
      }
      break;

   case typeExpression:
      {
         std::ostringstream buffer;
         buffer <<
            (m_isProcessed ? "   // " : "") <<
            "expression: " <<
            expression_data.expression <<
            (expression_data.is_address ? " (address-of)" : " (value-of)") <<
            "; level=" << expression_data.prec_level;
         item_text = buffer.str();
      }
      break;

   default:
      UaAssert(false);
      item_text = "// unknown graph item type!";
      break;
   }

   return item_text;
}

std::string CodeGraphItem::FormatOpcode() const
{
   UaAssert(m_type == typeOpcode);

   std::ostringstream buffer;
   buffer << "   ";

   if (m_isProcessed)
      buffer << "// ";

   // code segment address
   buffer << std::setfill('0') << std::setw(4) <<
      std::setbase(16) << m_pos << " ";

   if (opcode_data.opcode > op_last)
   {
      // unknown opcode
      buffer << "??? (0x" << std::setfill('0') <<
         std::setw(4) << std::setbase(16) << opcode_data.opcode << ")";
   }
   else
   {
      Uint16 opcode = opcode_data.opcode;

      buffer << g_aInstructions[opcode].mnemonic;

      if (g_aInstructions[opcode_data.opcode].args > 0 &&
         opcode_data.jump_target.size() > 0 &&
         (opcode == op_JMP || opcode == op_BEQ || opcode == op_BNE ||
         opcode == op_BRA || opcode == op_CALL))
      {
         // label available
         buffer << " " << opcode_data.jump_target;
      }
      else
         if (opcode == op_CALLI)
         {
            // intrinsic function name
            buffer << " " << opcode_data.jump_target;
         }
         else
            if (g_aInstructions[opcode_data.opcode].args > 0)
            {
               // unknown, not resolved by collect_xrefs, or PUSH, PUSHI_EFF
               // just format the string
               buffer << " 0x" << std::setfill('0') <<
                  std::setw(4) << std::setbase(16) << opcode_data.arg;
            }
   }

   return buffer.str();
}

//
// CodeGraph
//

CodeGraph::CodeGraph(const std::vector<Uint16>& code,
   Uint16 codestart, Uint16 codeend,
   std::vector<std::string>& strings,
   std::map<Uint16, ImportedItem>& imported_funcs,
   std::map<Uint16, ImportedItem>& imported_vars, Uint16 uiNumGlobals)
:m_strings(strings),
m_imported_funcs(imported_funcs),
m_imported_vars(imported_vars),
m_uiNumGlobals(uiNumGlobals)
{
   InitGraph(code, codestart, codeend);
}

void CodeGraph::Disassemble()
{
   CollectXrefs();
}

void CodeGraph::Decompile()
{
   CollectXrefs();
   FindFunctions();
   UpdateCallJumpTargets();

   // initially add main() to analysis queue
   m_analysis_queue.push_back("main");

   ProcessFunctionQueue();

   MarkFunctionsUnused();
}

// private functions

void CodeGraph::InitGraph(const std::vector<Uint16>& code,
   Uint16 codestart, Uint16 codeend)
{

   // convert code segment to graph
   for (Uint16 i = codestart; i<codeend; i++)
   {
      // fill item struct
      CodeGraphItem item;
      item.m_pos = i;
      item.opcode_data.opcode = code[i];

      if (item.opcode_data.opcode <= op_last &&
         g_aInstructions[item.opcode_data.opcode].args != 0)
         item.opcode_data.arg = code[++i];
      else
         item.opcode_data.arg = 0;

      // add it to graph
      m_graph.push_back(item);
   }
}

void CodeGraph::CollectXrefs()
{
   UaAssert(!m_graph.empty());

   // start function
   CodeGraphItem& startItem = m_graph.front();
   startItem.m_labelName = "start";
   startItem.m_xrefCount = 1;

   T_GraphIterator iter = m_graph.begin(), stop = m_graph.end();

   for (; iter != stop; iter++)
   {
      CodeGraphItem& item = *iter;
      if (item.m_type != typeOpcode)
         continue;

      Uint16 opcode = item.opcode_data.opcode;

      if (opcode == op_JMP ||
         opcode == op_BEQ ||
         opcode == op_BNE ||
         opcode == op_BRA ||
         opcode == op_CALL)
      {
         // calculate jump target
         Uint16 target = item.opcode_data.arg;

         if (target == 0x09d9)
            _asm nop;

         // adjust for relative jumps
         if (opcode == op_BEQ ||
            opcode == op_BNE ||
            opcode == op_BRA)
            target = static_cast<Uint16>((static_cast<Uint32>(target)+static_cast<Uint32>(item.m_pos + 1)) & 0xFFFF);

         // find target position
         T_GraphIterator target_pos = FindPos(target);
         UaAssert(target_pos != m_graph.end());

         CodeGraphItem& target_item = *target_pos;

         if (target_item.m_xrefCount == 0)
         {
            // not a target yet, generate label name from pos
            std::ostringstream buffer;
            buffer << "label_" << std::setfill('0') <<
               std::setw(4) << std::setbase(16) << target_item.m_pos;
            target_item.m_labelName = buffer.str();
         }

         // copy over label name and pos
         item.opcode_data.jump_target = target_item.m_labelName;
         item.opcode_data.jump_target_pos = target_item.m_pos;

         // increase xref count
         target_item.m_xrefCount++;
      }

      if (opcode == op_CALLI)
      {
         // for intrinsic calls, set target name
         Uint16 ifunc = item.opcode_data.arg;
         UaAssert(m_imported_funcs.find(ifunc) != m_imported_funcs.end());

         item.opcode_data.jump_target = m_imported_funcs.find(ifunc)->second.m_strName;
      }
   }
}

/// Searches for function entry and exit code and inserts additional graph items.
///
/// function entry code:
/// PUSHBP
/// SPTOBP
/// PUSHI #n   // n: locals size
/// ADDSP
///
/// and function exit code:
/// BPTOSP
/// POPBP
/// RET
void CodeGraph::FindFunctions()
{
   T_GraphIterator iter = m_graph.begin(), stop = m_graph.end();

   while (iter != stop)
   {
      FuncInfo funcInfo;

      if (!FindFunctionEntryPoint(iter, stop, funcInfo))
         break;

      if (!FindFunctionExitPoint(iter, stop, funcInfo))
         break;

      m_functionMap.insert(std::make_pair(funcInfo.name, funcInfo));
   }
}

bool CodeGraph::FindFunctionEntryPoint(T_GraphIterator& iter, T_GraphIterator stop, FuncInfo& funcInfo)
{
   // find next function entry code
   const Uint16 pattern_func_start[4] =
   { op_PUSHBP, op_SPTOBP, op_PUSHI, op_ADDSP };

   T_GraphIterator found = FindOpcodePattern(iter, stop, pattern_func_start, 4);

   if (found == stop)
      return false; // no more functions

   // found entry code
   iter = found;

   // set up new function start item
   CodeGraphItem& opcode_item = *iter;

   CodeGraphItem func_item;
   func_item.m_type = typeFuncStart;
   func_item.m_pos = opcode_item.m_pos;

   // find out number of local variables
   T_GraphIterator iter_pushi = iter;
   std::advance(iter_pushi, 2);
   Uint16 numLocals = iter_pushi->opcode_data.arg;

   funcInfo.locals_types.resize(numLocals);

   // generate function name from label, if applicable
   if (opcode_item.m_pos == 0 || opcode_item.m_pos == 1)
   {
      opcode_item.m_labelName = "main";
      opcode_item.m_xrefCount = 1;
   }
   else
      if (opcode_item.m_labelName.size() == 0 && opcode_item.m_xrefCount == 0)
      {
         // unused func
         std::ostringstream buffer;
         buffer << "unused_" << std::setfill('0') <<
            std::setw(4) << std::setbase(16) << opcode_item.m_pos;
         opcode_item.m_labelName = buffer.str();
      }
      else
         if (opcode_item.m_labelName.find("label_") == 0)
         {
            // label to func
            opcode_item.m_labelName.replace(0, 6, "func_");
         }

   func_item.m_labelName = opcode_item.m_labelName;
   opcode_item.m_labelName = "";

   func_item.m_xrefCount = opcode_item.m_xrefCount;
   opcode_item.m_xrefCount = 0;

   // test if function has START opcode before this one
   bool func_start = false;
   if (iter != m_graph.begin())
   {
      T_GraphIterator iter_start = iter;
      iter_start--;

      func_start = iter_start->m_type == typeOpcode &&
         iter_start->opcode_data.opcode == op_START;

      if (func_start)
      {
         --iter;
         --func_item.m_pos;
      }
   }

   // insert new func item
   m_graph.insert(iter, func_item); // insert before iter

   // advance iterator
   SetOpcodesProcessed(iter, stop, func_start ? 5 : 4);

   funcInfo.start = func_item.m_pos;
   funcInfo.name = func_item.m_labelName;

   return true;
}

bool CodeGraph::FindFunctionExitPoint(T_GraphIterator& iter, T_GraphIterator stop, FuncInfo& funcInfo)
{
   const Uint16 pattern_func_end[3] = { op_BPTOSP, op_POPBP, op_RET };
   T_GraphIterator found = FindOpcodePattern(iter, stop, pattern_func_end, 3);

   if (found == stop)
      return false;

   iter = found;

   // set up new function end item
   CodeGraphItem& end_opcode_item = *iter;

   CodeGraphItem end_item;
   end_item.m_type = typeFuncEnd;
   end_item.m_pos = end_opcode_item.m_pos;

   // transer xref count and label
   end_item.m_xrefCount = end_opcode_item.m_xrefCount;
   end_opcode_item.m_xrefCount = 0;

   end_item.m_labelName = end_opcode_item.m_labelName;

   m_graph.insert(iter, end_item);

   SetOpcodesProcessed(iter, stop, 3);

   funcInfo.end = end_item.m_pos;

   return true;
}

void CodeGraph::UpdateCallJumpTargets()
{
   T_GraphIterator iter = m_graph.begin(), stop = m_graph.end();

   // go through code and search for functions
   for (; iter != stop; iter++)
   {
      // correct jump targets for opcodes here
      if (iter->m_type == typeOpcode && iter->opcode_data.opcode == op_CALL)
      {
         T_GraphIterator target_iter = FindPos(iter->opcode_data.arg);
         iter->opcode_data.jump_target = target_iter->m_labelName;
      }
   }
}

void CodeGraph::ProcessFunctionQueue()
{
   // the queue may grow while analyzing a function
   while (!m_analysis_queue.empty())
   {
      std::string funcName = m_analysis_queue.front();
      m_analysis_queue.pop_front();

      if (m_functionMap.find(funcName) == m_functionMap.end())
      {
         UaAssert(false);
         continue; // function not found!
      }

      FuncInfo& funcInfo = m_functionMap[funcName];

      if (m_processed_funcs.find(funcInfo.start) != m_processed_funcs.end())
         continue; // already analyzed

      m_processed_funcs.insert(funcInfo.start);

      T_GraphIterator iter = FindPos(funcInfo.start);

      ATLTRACE(_T("analyzing function \"%hs\" at %04x\n"),
         funcName.c_str(), funcInfo.start);

      AnalyzeFunction(funcInfo);

      CombineOperators(funcInfo);

      FindSwitchCase(funcInfo);

      FindWhile(funcInfo);

      FindIfElse(funcInfo);

      AddGotoJumps(funcInfo);

      PostProcessFunction(funcInfo);
   }
}

void CodeGraph::AnalyzeFunction(FuncInfo& funcInfo)
{
   T_GraphIterator iter = FindPos(funcInfo.start), stop = FindPos(funcInfo.end);

   if (iter == m_graph.end())
   {
      UaAssert(false);
      // opcode for function vanished?
      return;
   }

   // Go through the function and replace with expressions and operations;
   // Expressions are incomplete values on the stack.
   // Operations are complete control statements that may consume zero or more
   // expressions.
   for (; iter != stop; iter++)
   {
      // only examine opcodes that weren't processed yet
      if (iter->m_type != typeOpcode || iter->m_isProcessed)
         continue;

      // first, try some pattern we need to process before looking at single opcodes
      const Uint16 pattern_local_array[3] = { op_PUSHI, op_PUSHI_EFF, op_OFFSET };

      if (MatchOpcodePattern(iter, stop, pattern_local_array, 3))
      {
         AddLocalArrayExpression(iter, funcInfo);

         SetOpcodesProcessed(iter, stop, 3);

         std::advance(iter, 3 - 1);
         continue;
      }

      const Uint16 pattern_return_stmt[2] = { op_SAVE_REG, op_POP };

      if (MatchOpcodePattern(iter, stop, pattern_return_stmt, 2))
      {
         AddOperator(iter, 1, false, dataTypeVoid);

         SetOpcodesProcessed(iter, stop, 2);

         // remember return type
         funcInfo.return_type = dataTypeInt;

         std::advance(iter, 2 - 1);
         continue;
      }

      CodeGraphItem& item = *iter;
      Uint16 opcode = item.opcode_data.opcode;

      switch (opcode)
      {
      case op_SAY_OP:
         // consumes 1 expression, returns none
         AddOperator(iter, 1, false, dataTypeVoid);
         break;

      case op_EXIT_OP:
         AddStatement(iter, "exit;");
         break;

         // intrinsic function call
      case op_CALLI:
         AddCallOperator(iter, stop, funcInfo, true);
         break;

         // local function call
      case op_CALL:
         AddCallOperator(iter, stop, funcInfo, false);
         break;

         // compare/arithmetic/logical operators
      case op_TSTEQ:
      case op_TSTGT:
      case op_TSTGE:
      case op_TSTLT:
      case op_TSTLE:
      case op_TSTNE:
      case op_OPOR:
      case op_OPAND:
      case op_OPADD:
      case op_OPSUB:
      case op_OPMUL:
      case op_OPDIV:
      case op_OPMOD:
         // binary operator, returns one int expression
         AddOperator(iter, 2, true, dataTypeInt);
         break;

      case op_OPNEG: // unary negate
      case op_OPNOT: // logical not
         AddOperator(iter, 1, true, dataTypeInt);
         break;

         // assignment operator
      case op_STO:
         AddAssignmentOperator(iter);
         break;

         // address-of local var
      case op_PUSHI_EFF: // note: must be checked after pattern_local_array
         AddAddressOfLocalVarExpression(iter, stop, funcInfo);
         break;

         // immediate value
      case op_PUSHI:
         // check if CALLI follows; if yes, don't process this PUSHI
         {
            T_GraphIterator next_iter = iter;
            ++next_iter;
            if (IsOpcode(next_iter, op_CALLI))
               continue;
         }

         AddImmediateExpression(iter, stop);
         break;

         // dereference-operator
      case op_FETCHM:
         // turns an address-of variable to value-of variable
         AddOperator(iter, 1, true, dataTypeInt);
         break;

         // array-index-operator
      case op_OFFSET:
         AddOperator(iter, 2, true, dataTypeInt);
         break;
      }
   }
}

CodeGraphItem& CodeGraph::AddOperator(T_GraphIterator& iter,
   Uint16 uiNeededExpressions, bool bReturnsExpression, T_enDataType returnedType)
{
   CodeGraphItem& item = *iter;
   item.m_isProcessed = true;

   Uint16 opcode = item.opcode_data.opcode;

   CodeGraphItem operatorItem;
   operatorItem.m_type = typeOperator;
   operatorItem.m_pos = iter->m_pos;
   operatorItem.operator_data.op_opcode = opcode;
   operatorItem.operator_data.needed_expr = uiNeededExpressions;
   operatorItem.operator_data.returns_expr = bReturnsExpression;
   operatorItem.operator_data.returned_type = returnedType;
   operatorItem.operator_data.prec_level =
      g_aInstructions[opcode].op_prec_level;

   return *m_graph.insert(iter, operatorItem);
}

CodeGraphItem& CodeGraph::AddStatement(T_GraphIterator iter, std::string text, bool bSetProcessed)
{
   CodeGraphItem& item = *iter;

   if (bSetProcessed)
      item.m_isProcessed = true;

   CodeGraphItem statementItem;
   statementItem.m_type = typeStatement;
   statementItem.m_pos = iter->m_pos;
   statementItem.statement_data.statement = text;

   return *m_graph.insert(iter, statementItem);
}

CodeGraphItem& CodeGraph::AddExpression(T_GraphIterator& iter, std::string text, bool is_address)
{
   CodeGraphItem& item = *iter;
   item.m_isProcessed = true;

   CodeGraphItem expressionItem;
   expressionItem.m_type = typeExpression;
   expressionItem.m_pos = iter->m_pos;
   expressionItem.expression_data.expression = text;
   expressionItem.expression_data.is_address = is_address;

   if (!iter->m_labelName.empty())
   {
      expressionItem.m_labelName = iter->m_labelName;
      iter->m_labelName.clear();
   }

   return *m_graph.insert(iter, expressionItem);
}

void CodeGraph::AddLocalArrayExpression(T_GraphIterator iter, FuncInfo& funcInfo)
{
   // get array offset, from the op_PUSHI
   T_GraphIterator arr_iter = iter;
   UaAssert(arr_iter->m_type == typeOpcode && arr_iter->opcode_data.opcode == op_PUSHI);
   Uint16 offset = arr_iter->opcode_data.arg;

   // TODO arrays with offset == 0 really allowed?
   UaAssert(offset <= 0x7fff && offset >= 0);
   arr_iter++;

   // get local var index, from op_PUSHI_EFF
   UaAssert(arr_iter->m_type == typeOpcode && arr_iter->opcode_data.opcode == op_PUSHI_EFF);

   Uint16 localIndex = arr_iter->opcode_data.arg;
   arr_iter++;

   // add expression
   std::ostringstream buffer;
   buffer << "local_" << localIndex << "[" << offset << "]";

   AddExpression(iter, buffer.str(), true);

   AddArrayInfo(funcInfo, localIndex, offset);
}

void CodeGraph::AddArrayInfo(FuncInfo& funcInfo, Uint16 localIndex, Uint16 offset)
{
   unsigned int max = funcInfo.array_info.size();
   for (unsigned int i = 0; i<max; i++)
   {
      if (funcInfo.array_info[i].local_start == localIndex)
      {
         // found info; update array size
         ArrayInfo& array_info = funcInfo.array_info[i];
         array_info.array_size = std::max<unsigned int>(array_info.array_size, offset);

         return;
      }
   }

   // new array
   ArrayInfo array_info;
   array_info.local_start = localIndex;
   array_info.array_size = offset;

   funcInfo.array_info.push_back(array_info);
}

void CodeGraph::AddCallOperator(T_GraphIterator& iter, T_GraphIterator stop, const FuncInfo& funcInfo, bool bIntrinsic)
{
   iter->m_isProcessed = true;

   // note: when bIntrinsic == true, this is the intrinsic number
   //       when bIntrinsic == false, it is the local function pos
   Uint16 call_target = iter->opcode_data.arg;
   std::string target_name = iter->opcode_data.jump_target;

   if (bIntrinsic)
   {
      --iter; // previous PUSHI opcode tells number of arguments
      UaAssert(true == IsOpcode(iter, op_PUSHI));

      // set PUSHI opcode to "processed"
      iter->m_isProcessed = true;

      // Note: This number may be wrong, e.g. for babl_menu
      // therefore we do not use this, but do the POP counting below.
      //oper_item.control_data.needed_expr = iter->opcode_data.arg;
   }

   // find out number of function args by counting following POP opcodes
   Uint16 arguments = 0;

   T_GraphIterator pop_iter = iter;
   std::advance(pop_iter, bIntrinsic ? 2 : 1);

   while (pop_iter != stop && IsOpcode(pop_iter, op_POP))
   {
      pop_iter->m_isProcessed = true;
      ++arguments;
      ++pop_iter;
   }

   // check if a PUSH_REG follows
   bool bReturnValue = false;
   if (IsOpcode(pop_iter, op_PUSH_REG))
   {
      pop_iter->m_isProcessed = true;
      bReturnValue = true;
   }

   // note: CALLI functions have 2 parameter, but the 1st is the number
   // of arguments, so subtract 1 from this number again
   if (bIntrinsic)
      --arguments;

   // operator with "arguments" needed expressions
   CodeGraphItem& operatorItem = AddOperator(iter, arguments, bReturnValue,
      bReturnValue ? dataTypeInt : dataTypeVoid);

   // correct opcode
   if (bIntrinsic)
      operatorItem.operator_data.op_opcode = op_CALLI;

   operatorItem.operator_data.prec_level = g_aInstructions[op_CALL].op_prec_level;

   operatorItem.operator_data.op_arg = call_target;

   std::advance(iter, arguments + (bReturnValue ? 1 : 0) - 1);

   if (!bIntrinsic)
   {
      // is recursive call?
      if (call_target == funcInfo.start)
      {
         ATLTRACE("discovered recursive function call to %hs() at %04x\n",
            target_name.c_str(), iter->m_pos);
         return;
      }

      // add to list of caller
      UaAssert(m_functionMap.find(target_name) != m_functionMap.end());

      FuncInfo& calledFuncInfo = m_functionMap[target_name];
      calledFuncInfo.caller.insert(funcInfo.name);

      // put in analysis queue, when not already processed
      if (m_processed_funcs.find(call_target) == m_processed_funcs.end())
         m_analysis_queue.push_back(target_name);
   }
}

void CodeGraph::AddAssignmentOperator(T_GraphIterator iter)
{
   iter->m_isProcessed = true;

   // check if a SWAP precedes the STO opcode
   T_GraphIterator swap_iter = iter;
   UaAssert(iter != m_graph.begin());
   --swap_iter;

   bool bStoSwapArgs = false;
   if (IsOpcode(swap_iter, op_SWAP))
   {
      swap_iter->m_isProcessed = true;

      bStoSwapArgs = true;
   }

   CodeGraphItem& operatorItem =
      AddOperator(iter, 2, false, dataTypeVoid);

   operatorItem.operator_data.sto_swap_args = bStoSwapArgs;
}

void CodeGraph::AddAddressOfLocalVarExpression(T_GraphIterator& iter, T_GraphIterator stop, FuncInfo& funcInfo)
{
   // local var index
   Uint16 localIndex = iter->opcode_data.arg;
   iter->m_isProcessed = true;

   // set up expression item
   bool is_address = true;

   // check if a FETCHM follows; then it's not the address of the
   // local var but the value
   T_GraphIterator fetchm_iter = iter;
   ++fetchm_iter;
   UaAssert(fetchm_iter != m_graph.end());

   bool bIsFetchm = IsOpcode(fetchm_iter, op_FETCHM);
   if (bIsFetchm)
   {
      fetchm_iter->m_isProcessed = true;
      is_address = false;
   }

   std::ostringstream buffer;
   if (localIndex > 0x7fff)
   {
      unsigned int paramNum = (-(Sint16)localIndex) - 1;

      // param value
      buffer << "param" << paramNum;

      // this expression is an address, since param values are always
      // passed by reference
      is_address = true;

      // set param info
      if (paramNum > funcInfo.param_types.size())
         funcInfo.param_types.resize(paramNum, dataTypeInt);
   }
   else
   {
      buffer << "local_" << localIndex;

      // add local variable info
      if (localIndex >= funcInfo.locals_types.size())
         funcInfo.locals_types.resize(localIndex + 1, dataTypeUnknown);

      UaAssert(localIndex < funcInfo.locals_types.size());

      funcInfo.locals_types[localIndex] = dataTypeInt;
   }

   AddExpression(iter, buffer.str(), is_address);

   if (bIsFetchm)
      ++iter;
}

void CodeGraph::AddImmediateExpression(T_GraphIterator& iter, T_GraphIterator stop)
{
   iter->m_isProcessed = true;

   // check if a FETCHM follows; then it's not an immediate value,
   // but the value of a global var
   T_GraphIterator fetchm_iter = iter;
   ++fetchm_iter;
   UaAssert(fetchm_iter != m_graph.end());

   if (IsOpcode(fetchm_iter, op_FETCHM))
   {
      fetchm_iter->m_isProcessed = true;

      // global or private variable value
      Uint16 varIndex = iter->opcode_data.arg;

      std::string text = GetMemoryVarName(varIndex);

      AddExpression(iter, text, false);

      ++iter;
   }
   else
   {
      // might be an immediate (or a global address, which is resolved later)
      std::ostringstream buffer;
      buffer << iter->opcode_data.arg;

      CodeGraphItem& pushImmItem = AddExpression(iter, buffer.str(), false);

      pushImmItem.expression_data.pushi_imm_value = iter->opcode_data.arg;
      pushImmItem.expression_data.is_pushi_imm = true;
   }
}

void CodeGraph::CombineOperators(FuncInfo& funcInfo)
{
   T_GraphIterator start = FindPos(funcInfo.start),
      stop = FindPos(funcInfo.end);

   while (true)
   {
      // search next operator
      bool bFound = false;
      T_GraphIterator iter;
      for (iter = start; iter != stop; iter++)
      {
         if (iter->m_type == typeOperator && !iter->m_isProcessed)
         {
            // found operator
            bFound = true;
            break;
         }
      }

      if (!bFound)
         break; // no more operators

      // combine found operator with expressions to form new expression
      T_GraphIterator oper = iter;

      // check if a statement appears between this operator and the
      // expressions used, insert the new expression on the place the
      // operator is located; the reason for this is because uw1 conv.
      // code can use temporary local variables that are initialized and
      // used as parameter for function calls
      bool statement_between = false;

      // search N previous expressions
      std::vector<T_GraphIterator> expressions;

      if (oper->operator_data.needed_expr > 0)
      {
         CollectExpressions(start, oper,
            oper->operator_data.needed_expr, expressions, statement_between);
      }

      // when statement was in between, insert at operator place
      // when not: are there expressions? when not, insert at operator place
      // else insert at first expression place
      T_GraphIterator insert_iter = oper;
      if (!statement_between)
      {
         // since expressions are sorted later-to-earlier, use last iter as insert point
         if (!expressions.empty())
            insert_iter = expressions.back();
      }

      CombineOperatorAndExpressions(oper, stop, insert_iter, expressions);

      // transfer label from fist expression
      if (!expressions.empty())
      {
         T_GraphIterator last_expr_iter = expressions.back();
         if (!last_expr_iter->m_labelName.empty())
         {
            insert_iter->m_labelName = last_expr_iter->m_labelName;
            last_expr_iter->m_labelName.clear();
         }
      }

      // mark operator as "done"
      oper->m_isProcessed = true;

      // mark expressions as "done"
      unsigned int max = expressions.size();
      for (unsigned int i = 0; i<max; i++)
         expressions[i]->m_isProcessed = true;
   }
}

void CodeGraph::CollectExpressions(T_GraphIterator start, T_GraphIterator end,
   unsigned int needed_expr, std::vector<T_GraphIterator>& expressions, bool& statement_between)
{
   T_GraphIterator iter = end;
   --iter;
   while (iter != start && expressions.size() < needed_expr)
   {
      if (iter->m_type == typeExpression && !iter->m_isProcessed)
      {
         // found an expression
         expressions.push_back(iter);
      }

      // remember if there was a statement
      // uw conv code supports something like "in-parameter value setting" like this:
      // get_quest(local_123 = 4);
      // when this is the case, remember that there were statements between
      if (iter->m_type == typeStatement)
         statement_between = true;

      --iter;
   }

   // not enough expressions found between operator and start of function?
   UaAssert(iter != start);
   UaAssert(expressions.size() == end->operator_data.needed_expr);
}

void CodeGraph::CombineOperatorAndExpressions(T_GraphIterator oper, T_GraphIterator stop,
   T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   switch (oper->operator_data.op_opcode)
   {
   case op_CALL:
   case op_CALLI:
      CombineCallOperator(oper, insert_iter, expressions);
      break;

      // create a "return" statement
   case op_SAVE_REG:
      CombineReturnExpression(insert_iter, expressions);
      break;

   case op_FETCHM:
      CombineDereferenceExpression(insert_iter, expressions);
      break;

   case op_OFFSET:
      CombineArrayExpression(insert_iter, expressions);
      break;

      // comparison operators
   case op_TSTEQ:
   case op_TSTNE:
   case op_TSTGT:
   case op_TSTGE:
   case op_TSTLT:
   case op_TSTLE:
      // arithmetic and logical operators
   case op_OPADD:
   case op_OPMUL:
   case op_OPSUB:
   case op_OPDIV:
   case op_OPMOD:
   case op_OPOR:
   case op_OPAND:
      AddBinaryExpression(oper, insert_iter, expressions);
      break;

   case op_OPNEG:
      AddUnaryExpression(oper, insert_iter, expressions, "-");
      break;

   case op_OPNOT:
      AddUnaryExpression(oper, insert_iter, expressions, "!");
      break;

   case op_SAY_OP:
      CombineSayOp(insert_iter, expressions);
      break;

   case op_STO:
      CombineAssignmentExpressions(oper, stop, insert_iter, expressions);
      break;

   default:
      // opcode appeared that wasn't processed
      UaAssert(false);
      break;
   }
}

void CodeGraph::CombineCallOperator(T_GraphIterator oper,
   T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   std::ostringstream buffer;

   bool bIntrinsic = oper->operator_data.op_opcode == op_CALLI;
   if (bIntrinsic)
   {
      // intrinsic
      Uint16 val = oper->operator_data.op_arg;
      UaAssert(m_imported_funcs.find(val) != m_imported_funcs.end());

      buffer << m_imported_funcs.find(val)->second.m_strName;
   }
   else
   {
      buffer << "func_" << std::setfill('0') <<
         std::setw(4) << std::setbase(16) << oper->operator_data.op_arg;
   }

   buffer << "(";

   // do parameter
   unsigned int max = expressions.size();
   for (unsigned int n = 0; n<max; n++)
   {
      CodeGraphItem& param = *expressions[n];

      // check if lvalue is from PUSHI opcode
      if (param.expression_data.is_pushi_imm)
         PushiImmediateToGlobal(expressions[n]);

      // param must be address, since all params are passed by reference/pointer
      UaAssert(true == param.expression_data.is_address);

      buffer << param.expression_data.expression;

      if (n < max - 1)
         buffer << ", ";
   }

   // decide if function call returns data
   if (oper->operator_data.returns_expr)
   {
      buffer << ")";

      AddExpression(insert_iter, buffer.str(), false);
   }
   else
   {
      buffer << ");";

      AddStatement(insert_iter, buffer.str());
   }
}

void CodeGraph::CombineReturnExpression(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   UaAssert(expressions.size() == 1);
   UaAssert(false == expressions[0]->expression_data.is_address);

   std::ostringstream buffer;
   buffer << "return " <<
      expressions[0]->expression_data.expression << ";";

   AddStatement(insert_iter, buffer.str());
}

void CodeGraph::CombineDereferenceExpression(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   UaAssert(expressions.size() == 1);
   UaAssert(true == expressions[0]->expression_data.is_address);

   CodeGraphItem& newExpression = AddExpression(insert_iter, "", false);
   newExpression.expression_data = expressions[0]->expression_data;
   newExpression.expression_data.is_address = false;
}

void CodeGraph::CombineArrayExpression(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   UaAssert(expressions.size() == 2);

   // check if expression 0 is from PUSHI opcode
   if (expressions[0]->expression_data.is_pushi_imm)
      PushiImmediateToGlobal(expressions[0]);

   UaAssert(true == expressions[0]->expression_data.is_address);
   UaAssert(false == expressions[1]->expression_data.is_address);

   std::ostringstream buffer;

   buffer << expressions[0]->expression_data.expression <<
      "[" << expressions[1]->expression_data.expression << "]";

   AddExpression(insert_iter, buffer.str(), true);
}

void CodeGraph::AddBinaryExpression(T_GraphIterator oper,
   T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   UaAssert(expressions.size() == 2);

   T_GraphIterator rvalue, lvalue;
   rvalue = expressions[0];
   lvalue = expressions[1];

   // check special case: left side of equality op is immediate value
   if (oper->operator_data.op_opcode == op_TSTEQ)
   {
      if (lvalue->expression_data.is_pushi_imm)
         std::swap(rvalue, lvalue);
   }

   unsigned int precedence = oper->operator_data.prec_level;

   // check that both lvalue and rvalue are value-of
   UaAssert(false == lvalue->expression_data.is_address);
   UaAssert(false == rvalue->expression_data.is_address);

   // when an expression used has a higher precedence, put
   // parenthesis around that expression
   std::ostringstream buffer;

   // lvalue
   if (precedence > lvalue->expression_data.prec_level)
      buffer << "(" << lvalue->expression_data.expression << ")";
   else
      buffer << lvalue->expression_data.expression;

   // operator character
   buffer << " " << g_aInstructions[oper->operator_data.op_opcode].operator_text << " ";

   // rvalue
   if (precedence > rvalue->expression_data.prec_level)
      buffer << "(" << rvalue->expression_data.expression << ")";
   else
      buffer << rvalue->expression_data.expression;

   CodeGraphItem& newExpression = AddExpression(insert_iter, buffer.str(), false);

   // the new expression has the lowest precedence of all used expressions
   unsigned int new_precedence = std::min(expressions[0]->expression_data.prec_level,
      expressions[1]->expression_data.prec_level);
   new_precedence = std::min(new_precedence, precedence);

   newExpression.expression_data.prec_level = new_precedence;
}

void CodeGraph::AddUnaryExpression(T_GraphIterator oper,
   T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions, std::string prefix)
{
   UaAssert(expressions.size() == 1);
   UaAssert(false == expressions[0]->expression_data.is_address);

   CodeGraphItem& newExpression = AddExpression(insert_iter, "", false);

   newExpression.expression_data = expressions[0]->expression_data;

   std::string newtext = prefix + newExpression.expression_data.expression;

   newExpression.expression_data.expression = newtext;
}

void CodeGraph::CombineSayOp(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   UaAssert(expressions.size() == 1);

   // add statement
   // SAY with PUSHI before?
   UaAssert(true == expressions[0]->expression_data.is_pushi_imm);

   Uint16 string_id = expressions[0]->expression_data.pushi_imm_value;
   UaAssert(string_id < m_strings.size());

   // replace linefeeds in text
   std::string outtext(m_strings[string_id]);

   std::string::size_type pos = 0;
   while (std::string::npos != (pos = outtext.find('\n')))
      outtext.replace(pos, 1, "\\n");

   std::ostringstream buffer;
   buffer << "say(\"" << outtext << "\");";

   AddStatement(insert_iter, buffer.str());
}

void CodeGraph::CombineAssignmentExpressions(T_GraphIterator oper, T_GraphIterator stop,
   T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions)
{
   UaAssert(expressions.size() == 2);

   T_GraphIterator rvalue, lvalue;
   rvalue = expressions[0];
   lvalue = expressions[1];

   // swap iterators if SWAP opcode was found
   if (oper->operator_data.sto_swap_args)
      std::swap(rvalue, lvalue);

   // check if lvalue is from PUSHI opcode
   if (lvalue->expression_data.is_pushi_imm)
      PushiImmediateToGlobal(lvalue);

   // check if assignment is for call to babl_menu or babl_fmenu
   CheckBablMenu(oper, stop, lvalue, rvalue);

   // check that lvalue is address-of and rvalue is value-of
   UaAssert(true == lvalue->expression_data.is_address);
   UaAssert(false == rvalue->expression_data.is_address);

   std::ostringstream buffer;
   buffer << lvalue->expression_data.expression << " = " <<
      rvalue->expression_data.expression << ";";

   AddStatement(insert_iter, buffer.str());
}

void CodeGraph::CheckBablMenu(T_GraphIterator& start,
   const T_GraphIterator& stop, T_GraphIterator& lvalue, T_GraphIterator& rvalue)
{
   UaAssert(rvalue->m_type == typeExpression);
   UaAssert(lvalue->m_type == typeExpression);

   if (!rvalue->expression_data.is_pushi_imm ||
      lvalue->expression_data.expression.find("[") == std::string::npos)
      return;

   T_GraphIterator iter = start;
   while (iter != stop)
   {
      // search for intrinsic calls
      if (iter->m_type == typeOperator &&
          iter->operator_data.op_opcode == op_CALLI)
         break; // found

      iter++;
   }

   if (iter == stop)
      return; // no operator with intrinsic found before end

   // check for babl_menu and babl_fmenu calls
   std::string intr_name =
      m_imported_funcs.find(iter->operator_data.op_arg)->second.m_strName;

   if (intr_name != "babl_menu" && intr_name != "babl_fmenu")
      return;

   if (intr_name == "babl_fmenu")
   {
      // check that lvalue array value matches arg0 of babl_fmenu
      T_GraphIterator prev_iter = iter;
      --prev_iter;
      if (prev_iter->m_type == typeOpcode &&
         prev_iter->opcode_data.opcode == op_PUSHI_EFF)
      {
         std::ostringstream buffer;
         buffer << "local_" << prev_iter->opcode_data.arg << "[";

         if (lvalue->expression_data.expression.find(buffer.str()) == std::string::npos)
            return; // not an expression for the "strings array" argument of babl_fmenu
      }
   }

   // determine string id and format string
   Uint16 str_id = rvalue->expression_data.pushi_imm_value;

   if (str_id >= m_strings.size())
      return; // invalid string id

   UaAssert(str_id < m_strings.size());

   if (str_id > 0)
   {
      std::ostringstream buffer;
      buffer << "\"" << m_strings[str_id].c_str() << "\""
         << "; // " << str_id;

      rvalue->expression_data.expression = buffer.str();
      rvalue->expression_data.is_pushi_imm = false;
   }
}

void CodeGraph::FindSwitchCase(FuncInfo& funcInfo)
{
   T_GraphIterator iter = FindPos(funcInfo.start),
      stop = FindPos(funcInfo.end);

   // find open expression
   for (; iter != stop; iter++)
   {
      bool bIsExpression = iter->m_type == typeExpression && !iter->m_isProcessed;
      if (!bIsExpression)
         continue;

      bool bSwitchAdded = false;

      Uint16 switch_end = 0;

      T_GraphIterator expr_iter = iter;

      do
      {
         bool bRet = FindAndAddNextSwitchCase(expr_iter, stop, switch_end, bSwitchAdded);
         if (!bRet)
            break;

         // check if next expression is an expression
         if (expr_iter->m_type != typeExpression || expr_iter->m_isProcessed == true)
         {
            if (bSwitchAdded)
            {
               T_GraphIterator switch_end_iter = expr_iter;
               --switch_end_iter;
               AddStatement(switch_end_iter, "} // end-switch", false);

               break;
            }
         }

      } while (switch_end != 0 && expr_iter->m_pos < switch_end);
   }
}

bool CodeGraph::FindAndAddNextSwitchCase(T_GraphIterator& expr_iter, T_GraphIterator stop, Uint16& switch_end, bool& bSwitchAdded)
{
   // a BEQ opcode must follow
   T_GraphIterator beq_iter = expr_iter;
   for (; beq_iter != stop; beq_iter++)
   {
      if (!beq_iter->m_isProcessed && beq_iter->m_type == typeOpcode)
      {
         if (beq_iter->opcode_data.opcode == op_BEQ)
            break; // found
         else
         {
            beq_iter = stop;
            break;
         }
      }
   }

   if (beq_iter == stop)
      return false; // no BEQ found, try next expression

   // find next JMP opcode, which is the break statement
   T_GraphIterator break_iter = beq_iter;
   break_iter++;
   for (; break_iter != stop; break_iter++)
   {
      if (!break_iter->m_isProcessed && break_iter->m_type == typeOpcode)
      {
         if (break_iter->opcode_data.opcode == op_JMP)
            break; // found
         else
         {
            break_iter = stop; // other opcode; give up
            break;
         }
      }
   }

   if (break_iter == stop)
      return false; // no JMP found, try next expression

   // check if there is a second JMP immediately after this
   T_GraphIterator second_jmp_iter = break_iter;
   ++second_jmp_iter;
   if (second_jmp_iter != stop &&
       !second_jmp_iter->m_isProcessed &&
       second_jmp_iter->m_type == typeOpcode &&
       second_jmp_iter->opcode_data.opcode == op_JMP)
   {
      // is this really a "goto" jump? or fall-through?

      // check if both jump targets are equal
      if (break_iter->opcode_data.jump_target_pos != second_jmp_iter->opcode_data.jump_target_pos)
      {
         std::ostringstream buffer;
         buffer << "goto " << break_iter->opcode_data.jump_target << ";";

         AddStatement(break_iter, buffer.str());
      }
      else
         break_iter->m_isProcessed = true;

      break_iter++;
   }

   // when first call, check if target of BEQ is an expression
   if (!bSwitchAdded)
   {
      T_GraphIterator next_expr_iter = FindPos(beq_iter->opcode_data.jump_target_pos);

      if (next_expr_iter->m_type != typeExpression || next_expr_iter->m_isProcessed)
         return false; // no expression; maybe we found an if without else statement
   }

   if (switch_end == 0)
      switch_end = break_iter->opcode_data.jump_target_pos;

   T_GraphIterator case_iter = expr_iter;

   // insert switch statement
   std::string expression = expr_iter->expression_data.expression;
   if (!bSwitchAdded)
   {
      std::string::size_type pos = expression.find(" == ");
      if (pos == std::string::npos)
      {
         // no expression "variable == value" found; give up
         return false;
      }

      {
         std::ostringstream buffer;
         buffer << "switch (" << expression.substr(0, pos) << ") {";

         AddStatement(expr_iter, buffer.str());
      }

      bSwitchAdded = true;

      case_iter++;
   }

   // add case statement
   {
      std::string::size_type pos = expression.find(" == ");
      std::string case_value = expression.substr(pos + 4);

      std::ostringstream buffer;
      buffer << "case " << case_value << ":";

      CodeGraphItem& caseStatement = AddStatement(case_iter, buffer.str());
      caseStatement.statement_data.indent_change_after = 1;
   }

   case_iter->m_isProcessed = true;
   beq_iter->m_isProcessed = true;

   // add break statement
   CodeGraphItem& breakStatement = AddStatement(break_iter, "break;");
   breakStatement.statement_data.indent_change_after = -1;

   // jump to next block via beq_iter
   expr_iter = FindPos(beq_iter->opcode_data.jump_target_pos);

   return true;
}

void CodeGraph::FindWhile(FuncInfo& funcInfo)
{
   T_GraphIterator iter = FindPos(funcInfo.start),
      stop = FindPos(funcInfo.end);

   // find open expression
   for (; iter != stop; iter++)
   {
      bool bIsExpression = iter->m_type == typeExpression && !iter->m_isProcessed;
      if (!bIsExpression)
         continue;

      T_GraphIterator expr_iter = iter;

      // a BEQ opcode must follow
      T_GraphIterator beq_iter = expr_iter;
      for (; beq_iter != stop; beq_iter++)
      {
         if (!beq_iter->m_isProcessed && beq_iter->m_type == typeOpcode)
         {
            if (beq_iter->opcode_data.opcode == op_BEQ)
               break; // found
            else
            {
               beq_iter = stop;
               break;
            }
         }
      }

      if (beq_iter == stop)
         continue; // no BEQ found, try next expression

      // BEQ must jump down (when not, there is another control structure)
      UaAssert(beq_iter->m_pos < beq_iter->opcode_data.jump_target_pos);

      // check jump target

      // check if both jump targets are equal
      Uint16 target_pos = beq_iter->opcode_data.jump_target_pos;
      T_GraphIterator target_iter = FindPos(target_pos);

      // check if there is a BRA opcode before this one
      T_GraphIterator bra_iter = target_iter;
      --bra_iter;

      bool bIsBranch = !bra_iter->m_isProcessed && beq_iter->m_type == typeOpcode && bra_iter->opcode_data.opcode == op_BRA;
      if (!bIsBranch)
         continue;

      Uint16 bra_target_pos = bra_iter->opcode_data.jump_target_pos;

      if (bra_target_pos > target_pos)
         continue; // a BRA opcode, but jumps down; might be a if statement

      // check that BRA target points to our expression
      if (expr_iter->m_pos != bra_target_pos)
      {
         // The BRA target doesn't point to the expression used at
         // the start of the while() loop. That indicates a for()
         // statement which isn't parsed yet. The BRA opcode
         // instead jumps to the for loop break condition check.
         // Happens in conv 10.
         UaTrace("possible for() statement found on pos %04x\n",
            bra_target_pos);

         continue;
      }

      // add statements
      std::ostringstream buffer;
      buffer << "while (" << expr_iter->expression_data.expression << ") {";

      CodeGraphItem& whileStatement = AddStatement(expr_iter, buffer.str());
      whileStatement.statement_data.indent_change_after = 1;

      beq_iter->m_isProcessed = true;

      CodeGraphItem& whileEndStatement = AddStatement(bra_iter, "} // end-while");
      whileEndStatement.statement_data.indent_change_before = -1;
   }
}

void CodeGraph::FindIfElse(FuncInfo& funcInfo)
{
   T_GraphIterator iter = FindPos(funcInfo.start),
      stop = FindPos(funcInfo.end);

   // find open expression
   for (; iter != stop; iter++)
   {
      bool bIsExpression = iter->m_type == typeExpression && !iter->m_isProcessed;
      if (!bIsExpression)
         continue;

      T_GraphIterator expr_iter = iter;

      // a BEQ opcode must follow
      T_GraphIterator beq_iter = expr_iter;
      for (; beq_iter != stop; beq_iter++)
      {
         if (!beq_iter->m_isProcessed && beq_iter->m_type == typeOpcode)
         {
            if (beq_iter->opcode_data.opcode == op_BEQ)
               break; // found
            else
            {
               beq_iter = stop;
               break;
            }
         }
      }

      if (beq_iter == stop)
         continue; // no BEQ found, try next expression

      // BEQ must jump down (when not, there is another control structure)
      UaAssert(beq_iter->m_pos < beq_iter->opcode_data.jump_target_pos);

      // check jump target

      // check if both jump targets are equal
      Uint16 beq_target_pos = beq_iter->opcode_data.jump_target_pos;
      T_GraphIterator target_iter = FindPos(beq_target_pos);

      // check if there is a BRA or JMP opcode before this one
      T_GraphIterator bra_iter = target_iter;
      --bra_iter;

      T_GraphIterator else_iter = stop;
      T_GraphIterator endif_iter = stop;

      // sometimes code uses JMP instead of BRA; maybe with empty if body?
      bool bHaveElse = !bra_iter->m_isProcessed &&
         beq_iter->m_type == typeOpcode &&
         (bra_iter->opcode_data.opcode == op_BRA ||
          bra_iter->opcode_data.opcode == op_JMP);

      if (bHaveElse)
      {
         Uint16 bra_target_pos = bra_iter->opcode_data.jump_target_pos;

         if (bra_target_pos < beq_target_pos)
         {
            UaAssert(false); // should be discovered by FindWhile() already!
            continue; // a BRA opcode, but jumps up; might be a while statement
         }

         bra_iter->m_isProcessed = true;

         else_iter = bra_iter;
         else_iter++;

         endif_iter = FindPos(bra_target_pos);
      }
      else
      {
         endif_iter = target_iter;
      }

      beq_iter->m_isProcessed = true;

      // insert statements
      std::ostringstream buffer;
      buffer << "if (" << expr_iter->expression_data.expression << ") {";

      CodeGraphItem& ifStatement = AddStatement(expr_iter, buffer.str());
      ifStatement.statement_data.indent_change_after = 1;

      if (else_iter != stop)
      {
         CodeGraphItem& elseStatement = AddStatement(else_iter, "} else {", false);
         elseStatement.statement_data.indent_change_before = -1;
         elseStatement.statement_data.indent_change_after = 1;
      }

      CodeGraphItem& endifStatement = AddStatement(endif_iter, "} // end-if", false);
      endifStatement.statement_data.indent_change_before = -1;
   }
}

void CodeGraph::AddGotoJumps(FuncInfo& funcInfo)
{
   T_GraphIterator iter = FindPos(funcInfo.start),
      stop = FindPos(funcInfo.end);

   // find remaining opcodes
   for (; iter != stop; iter++)
   {
      // only examine opcodes that weren't processed yet
      if (iter->m_type != typeOpcode || iter->m_isProcessed)
         continue;

      if (iter->opcode_data.opcode == op_JMP)
      {
         std::ostringstream buffer;
         buffer << "goto " << iter->opcode_data.jump_target << ";";

         AddStatement(iter, buffer.str());

         T_GraphIterator target_iter = FindPos(iter->opcode_data.jump_target_pos);

         // check if a label was already inserted
         if (target_iter->m_type != typeStatement ||
            target_iter->statement_data.statement.find("label") != 0)
         {
            // insert goto target label
            std::ostringstream buffer2;
            buffer2 << iter->opcode_data.jump_target << ":;";

            CodeGraphItem& labelStatement = AddStatement(iter, buffer2.str());
            labelStatement.statement_data.indent_change_before = -1;
            labelStatement.statement_data.indent_change_after = 1;
         }
      }
      else
         UaAssert(false); // unhandled open opcode
   }
}

void CodeGraph::PostProcessFunction(FuncInfo& funcInfo)
{
   // get function start
   T_GraphIterator iter = FindPos(funcInfo.start);
   CodeGraphItem& startItem = *iter;
   UaAssert(startItem.m_type == typeFuncStart);

   std::ostringstream buffer;

   // return type
   buffer << DataTypeToString(funcInfo.return_type) << " ";

   // function name
   buffer << startItem.m_labelName << "(";

   // parameters
   const std::vector<T_enDataType>& param_types = funcInfo.param_types;

   unsigned int max = param_types.size();
   for (unsigned int i = 0; i<max; i++)
   {
      buffer << DataTypeToString(param_types[i]) << " param" << i + 1;
      if (i < max - 1)
         buffer << ", ";
   }

   buffer << ")";
   buffer << " // referenced " << startItem.m_xrefCount << " times";

   CodeGraphItem& startStatement = AddStatement(iter, buffer.str());
   startStatement.statement_data.indent_change_after = 0;

   // opening brace
   CodeGraphItem& funcBrace = AddStatement(iter, "{");
   funcBrace.statement_data.indent_change_after = 1;

   // add all locals and arrays
   max = funcInfo.locals_types.size();
   for (unsigned int j = 0; j<max; j++)
   {
      T_enDataType type = funcInfo.locals_types[j];

      if (type != dataTypeUnknown)
      {
         std::ostringstream buffer;
         buffer << DataTypeToString(type) << " local_" << j << ";";

         AddStatement(iter, buffer.str());
      }
   }

   // arrays
   max = funcInfo.array_info.size();
   for (unsigned int n = 0; n<max; n++)
   {
      ArrayInfo array_info = funcInfo.array_info[n];

      std::ostringstream buffer;

      UaAssert(array_info.local_start <= funcInfo.locals_types.size());

      buffer << DataTypeToString(funcInfo.locals_types[array_info.local_start]) <<
         " local_" << array_info.local_start <<
         "[" << array_info.array_size << "];";

      AddStatement(iter, buffer.str());
   }

   // add function end
   iter = FindPos(funcInfo.end);
   while (iter != m_graph.end() && iter->m_type != typeFuncEnd)
      iter++;

   CodeGraphItem& endStatement = AddStatement(iter++, "} // end-function");
   endStatement.statement_data.indent_change_before = -1;

   // empty line
   AddStatement(iter, "");
}

void CodeGraph::MarkFunctionsUnused()
{
   std::for_each(m_functionMap.begin(), m_functionMap.end(), [&](const T_mapFunctions::value_type& val){

      const FuncInfo& funcInfo = val.second;

      Uint16 startPos = funcInfo.start;
      if (m_processed_funcs.find(startPos) == m_processed_funcs.end())
      {
         if (funcInfo.name.find("unused_") == 0)
            return;

         // unused; mark with 0 xrefs
         T_GraphIterator iter = FindPos(funcInfo.start);
         UaAssert(iter != m_graph.end());

         ATLTRACE(_T("removing function %hs, it is never called\n"), funcInfo.name.c_str());
         iter->m_xrefCount = 0;
      }
   });
}

// helper functions

CodeGraph::T_GraphIterator CodeGraph::FindPos(Uint16 target)
{
   T_GraphIterator iter = m_graph.begin(), stop = m_graph.end();

   for (; iter != stop; iter++)
      if (iter->m_pos == target)
         return iter;

   // should not happen
   UaAssert(false);
   return stop;
}

bool CodeGraph::IsOpcode(T_ConstGraphIterator iter, Uint16 opcode) const
{
   return (iter->m_type == typeOpcode &&
      iter->opcode_data.opcode == opcode);
}

CodeGraph::T_GraphIterator CodeGraph::FindOpcodePattern(T_GraphIterator iter, T_GraphIterator stop,
   const Uint16* pattern, unsigned int length)
{
   unsigned int matchCount = 0;

   T_GraphIterator found = stop;

   for (; iter != stop; iter++)
   {
      if (iter->m_type != typeOpcode)
         continue;

      if (iter->opcode_data.opcode == pattern[matchCount])
      {
         if (matchCount == 0)
            found = iter;

         ++matchCount;

         if (matchCount == length)
            return found; // found! return start
      }
      else
      {
         matchCount = 0; // reset; not all pattern opcodes matched
         found = stop;
      }
   }

   return stop;
}

bool CodeGraph::MatchOpcodePattern(T_ConstGraphIterator iter, T_ConstGraphIterator stop,
   const Uint16* pattern, unsigned int length) const
{
   for (unsigned int i = 0; i<length; i++, iter++)
      if (iter == stop || iter->m_type != typeOpcode ||
         iter->opcode_data.opcode != pattern[i])
         return false;

   // all pattern bytes found, no end reached
   return true;
}

void CodeGraph::SetOpcodesProcessed(T_GraphIterator iter, T_GraphIterator stop,
   unsigned int uiNumOpcodes)
{
   for (unsigned int i = 0; i < uiNumOpcodes && iter != stop; iter++)
   {
      if (iter->m_type == typeOpcode)
      {
         iter->m_isProcessed = true;
         i++;
      }
   }
}

std::string CodeGraph::GetMemoryVarName(Uint16 memoryIndex) const
{
   UaAssert(memoryIndex < m_uiNumGlobals);

   std::map<Uint16, ImportedItem>::iterator iter =
      m_imported_vars.find(memoryIndex);

   if (iter != m_imported_vars.end())
   {
      return iter->second.m_strName;
   }
   else
   {
      std::ostringstream buffer;
      buffer << "global_" << memoryIndex;

      return buffer.str();
   }
}

void CodeGraph::PushiImmediateToGlobal(T_GraphIterator iter)
{
   UaAssert(iter->m_type == typeExpression);
   UaAssert(true == iter->expression_data.is_pushi_imm);
   UaAssert(false == iter->expression_data.is_address);

   // resolve to proper address
   Uint16 memoryIndex = iter->expression_data.pushi_imm_value;

   iter->expression_data.expression = GetMemoryVarName(memoryIndex);

   iter->expression_data.is_pushi_imm = false;
   iter->expression_data.pushi_imm_value = 0;
   iter->expression_data.is_address = true;
}

const char* CodeGraph::DataTypeToString(T_enDataType type)
{
   switch (type)
   {
   case dataTypeVoid: return "void";
   case dataTypeInt: return "int";
   case dataTypeString: return "string";
   default:
      UaAssert(false);
      return "unknown";
   }
}
