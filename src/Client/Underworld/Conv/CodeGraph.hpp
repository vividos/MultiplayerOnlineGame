//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Conv/CodeGraph.hpp Conversation code graph for decompiler
//
#pragma once

// includes
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <set>
#include "CodeVM.hpp"

namespace Conv
{

/// code graph item type types
enum T_enGraphItemType
{
   typeOpcode = 0,   ///< assembler opcode
   typeFuncStart,    ///< function start
   typeFuncEnd,      ///< function end
   typeOperator,     ///< operator that combines one or more expressions and may yield an expression
   typeStatement,    ///< complete statement that returns no value
   typeExpression,   ///< expression that yields exactly one value
};

// structs

/// code graph item
struct CodeGraphItem
{
   /// ctor
   CodeGraphItem()
      :m_type(typeOpcode),
      m_pos(0),
      m_xrefCount(0),
      m_isProcessed(false)
   {
   }

   /// type of this item
   T_enGraphItemType m_type;

   /// position in code segment
   Uint16 m_pos;

   /// label name, if any
   std::string m_labelName;

   /// number of times the label is jumped to/branched/called/used
   unsigned int m_xrefCount;

   /// indicates if item is already processed
   bool m_isProcessed;

   /// formats item suitable for display
   std::string Format() const;

   /// formats opcode (item must have type typeOpcode)
   std::string FormatOpcode() const;

   /// data for the typeOpcode graph item
   struct OpcodeData
   {
      /// ctor
      OpcodeData()
         :opcode(0),
         arg(0),
         jump_target_pos(0)
      {
      }

      /// opcode values
      Uint16 opcode, arg;

      /// jump target for branches, jumps and calls
      std::string jump_target;

      /// absolute position of jump target
      Uint16 jump_target_pos;
   };

   /// data for the gt_statement graph item
   struct StatementData
   {
      /// ctor
      StatementData()
         :indent_change_before(0),
         indent_change_after(0){}

      /// statement text
      std::string statement;

      /// suggested indent change before this line
      int indent_change_before;

      /// suggested indent change after this line
      int indent_change_after;
   };

   /// data for the gt_operator graph item
   struct OperatorData
   {
      /// ctor
      OperatorData()
         :op_opcode(0),
         op_arg(0),
         needed_expr(0),
         returns_expr(false),
         returned_type(dataTypeVoid),
         sto_swap_args(false),
         prec_level(0xff)
      {
      }

      /// operator opcode and argument
      Uint16 op_opcode, op_arg;

      /// number of expression this operator consumes
      Uint16 needed_expr;

      /// indicates if operator returns an expression
      bool returns_expr;

      /// return type of expression, if any
      T_enDataType returned_type;

      /// swap expressions on STO opcode?
      bool sto_swap_args;

      /// operator precedence level
      unsigned int prec_level;
   };

   /// data for the gt_expression graph item
   struct ExpressionData
   {
      /// ctor
      ExpressionData()
         :is_address(false),
         is_pushi_imm(false),
         pushi_imm_value(0),
         prec_level(0xff)
      {
      }

      /// expression text
      std::string expression;

      /// indicates if expression is an address to a memory location or a value
      bool is_address;

      /// indicates if expression comes from an PUSHI opcode
      bool is_pushi_imm;

      /// immediate value for PUSHI opcodes
      Uint16 pushi_imm_value;

      /// expression precedence level
      unsigned int prec_level;
   };

   OpcodeData opcode_data;
   StatementData statement_data;
   OperatorData operator_data;
   ExpressionData expression_data;
};

/// info about a local array in a function
struct ArrayInfo
{
   /// number of local variable with array start
   Uint16 local_start;

   /// size of array
   unsigned int array_size;
};

/// info about a function
struct FuncInfo
{
   /// ctor
   FuncInfo()
      :return_type(dataTypeVoid),
       start(0)
   {
   }

   /// function name
   std::string name;

   /// start in code, when no intrisic
   Uint16 start;

   /// start of func end in code
   Uint16 end;

   /// return type
   T_enDataType return_type;

   /// list of types of function parameters
   std::vector<T_enDataType> param_types;

   /// list of local variable types
   std::vector<T_enDataType> locals_types;

   /// array info
   std::vector<ArrayInfo> array_info;

   /// all caller functions
   std::set<std::string> caller;
};

// classes

class CodeGraph
{
public:
   /// list of graph items
   typedef std::list<CodeGraphItem> T_GraphList;

   /// graph item iterator
   typedef T_GraphList::iterator T_GraphIterator;

   /// const graph item iterator
   typedef T_GraphList::const_iterator T_ConstGraphIterator;

   /// map with all function infos, key is function name
   typedef std::map<std::string, FuncInfo> T_mapFunctions;

public:
   /// ctor
   CodeGraph(const std::vector<Uint16>& code,
      Uint16 codestart, Uint16 codeend,
      std::vector<std::string>& strings,
      std::map<Uint16, ImportedItem>& imported_funcs,
      std::map<Uint16, ImportedItem>& imported_vars,
      Uint16 uiNumGlobals);

   /// returns graph
   const T_GraphList& GetGraph() const { return m_graph; }

   /// returns function map
   const T_mapFunctions& GetFunctionMap() const { return m_functionMap; }

   /// only disassembles conversation code
   void Disassemble();

   /// decompiles conversation code
   void Decompile();

private:
   /// initializes graph with opcodes
   void InitGraph(const std::vector<Uint16>& code,
      Uint16 codestart, Uint16 codeend);

   /// collect all call, branch and jump references
   void CollectXrefs();

   /// finds function starts and ends
   void FindFunctions();

   /// finds function entry point code
   bool FindFunctionEntryPoint(T_GraphIterator& iter, T_GraphIterator stop, FuncInfo& funcInfo);

   /// finds function exit code
   bool FindFunctionExitPoint(T_GraphIterator& iter, T_GraphIterator stop, FuncInfo& funcInfo);

   /// updates CALL opcode jump targets
   void UpdateCallJumpTargets();

   /// analyze all functions in the analysis queue
   void ProcessFunctionQueue();

   /// analyzes function
   void AnalyzeFunction(FuncInfo& funcInfo);

   // operator / expression analysis functions

   /// adds operator for given opcode item
   CodeGraphItem& AddOperator(T_GraphIterator& iter,
      Uint16 uiNeededExpressions, bool bReturnsExpression, T_enDataType returnedType);

   /// adds statement for given opcode item
   CodeGraphItem& AddStatement(T_GraphIterator iter, std::string text, bool bSetProcessed = true);

   /// adds a generic expression
   CodeGraphItem& AddExpression(T_GraphIterator& iter, std::string text, bool is_address);

   /// adds expression for local array access
   void AddLocalArrayExpression(T_GraphIterator iter, FuncInfo& funcInfo);

   /// adds array access info for given function
   void AddArrayInfo(FuncInfo& info, Uint16 localIndex, Uint16 offset);

   /// adds call operator
   void AddCallOperator(T_GraphIterator& iter, T_GraphIterator stop,
      const FuncInfo& funcInfo, bool bIntrinsic);

   /// adds assignment operator
   void AddAssignmentOperator(T_GraphIterator iter);

   /// adds expression for address-of local var
   void AddAddressOfLocalVarExpression(T_GraphIterator& iter, T_GraphIterator stop, FuncInfo& funcInfo);

   /// adds immediate expression (rvalue)
   void AddImmediateExpression(T_GraphIterator& iter, T_GraphIterator stop);

   /// combines operators with expressions
   void CombineOperators(FuncInfo& funcInfo);

   /// collects all expressions for an operator
   void CollectExpressions(T_GraphIterator start, T_GraphIterator end,
      unsigned int needed_expr, std::vector<T_GraphIterator>& expressions, bool& statement_between);

   /// combines operator with found expressions
   void CombineOperatorAndExpressions(T_GraphIterator oper, T_GraphIterator stop,
      T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// combines call operator with its parameters
   void CombineCallOperator(T_GraphIterator oper,
      T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// combines return operator with expression
   void CombineReturnExpression(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// combines an lvalue expression and dereferences it, creating a new expression
   void CombineDereferenceExpression(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// combines array and index expressions
   void CombineArrayExpression(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// combines arithmetical or logical operator expressions
   void AddBinaryExpression(T_GraphIterator oper,
      T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// combines unary arithmetical or logical operator with expression
   void AddUnaryExpression(T_GraphIterator oper,
      T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions, std::string prefix);

   /// combines say operator with expression
   void CombineSayOp(T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// combines assignment expression (lvalue and rvalue)
   void CombineAssignmentExpressions(T_GraphIterator oper, T_GraphIterator stop,
      T_GraphIterator insert_iter, std::vector<T_GraphIterator>& expressions);

   /// checks if an assignment expression may contain string assignment for babl_menu and babl_fmenu
   void CheckBablMenu(T_GraphIterator& start,
      const T_GraphIterator& stop, T_GraphIterator& lvalue, T_GraphIterator& rvalue);

   // statement analysis functions

   /// finds switch-case structures
   void FindSwitchCase(FuncInfo& funcInfo);

   /// adds next switch case statements
   bool FindAndAddNextSwitchCase(T_GraphIterator& expr_iter, T_GraphIterator stop, Uint16& switch_end, bool& bSwitchAdded);

   /// finds while statements
   void FindWhile(FuncInfo& funcInfo);

   /// finds if and if-else statements
   void FindIfElse(FuncInfo& funcInfo);

   /// adds goto and labels for remaining jumps
   void AddGotoJumps(FuncInfo& funcInfo);

   // post processing functions

   /// post processes function
   void PostProcessFunction(FuncInfo& funcInfo);

   /// marks functions unused, with only callers that are unused themselves
   void MarkFunctionsUnused();

   // helper functions

   /// finds first iterator for code position, or end() iterator
   T_GraphIterator FindPos(Uint16 target);

   /// checks if a given iter points to an opcode item and if it contains given opcode
   bool IsOpcode(T_ConstGraphIterator iter, Uint16 opcode) const;

   /// finds opcode pattern in given range
   T_GraphIterator FindOpcodePattern(T_GraphIterator iter, T_GraphIterator stop,
      const Uint16* pattern, unsigned int length);

   /// checks if next following opcodes match a given pattern
   bool MatchOpcodePattern(T_ConstGraphIterator iter, T_ConstGraphIterator stop,
      const Uint16* pattern, unsigned int length) const;

   /// sets following N opcodes to "processed"
   void SetOpcodesProcessed(T_GraphIterator iter, T_GraphIterator stop, unsigned int uiNumOpcodes);

   /// returns variable name of a global or imported variable
   std::string GetMemoryVarName(Uint16 memoryIndex) const;

   /// conver immediate value to global
   void PushiImmediateToGlobal(T_GraphIterator iter);

   /// converts data type to string
   static const char* DataTypeToString(T_enDataType type);

private:
   /// conversation graph
   T_GraphList m_graph;

   /// function map
   T_mapFunctions m_functionMap;

   /// queue of functions to analyze
   std::deque<std::string> m_analysis_queue;

   /// set with already processed functions (the entry points)
   std::set<Uint16> m_processed_funcs;

   /// number of globals on stack before imported vars
   Uint16 m_uiNumGlobals;

   /// all conversation strings
   std::vector<std::string>& m_strings;

   /// map with all intrinsic functions
   std::map<Uint16, ImportedItem>& m_imported_funcs;

   /// map with all imported variables
   std::map<Uint16, ImportedItem>& m_imported_vars;
};

} // namespace Conv
