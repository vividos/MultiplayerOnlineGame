//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Conv/CodeVM.hpp Conversation code virtual machine
//
#pragma once

// includes
#include <string>
#include <vector>
#include "Savegame.hpp"
#include "ConvStack.hpp"
#include "ConvGlobals.hpp"

/// for more info about the underworld conversation assembler script, look in
/// the file docs/uw-formats.txt
/// a gnu-gdb style debugger named "cnvdbg" and a disassembler and experimental
/// decompiler (produces C-like code) is available in the "tools" folder.

// forward references
namespace Underworld
{
class ConvGlobals;
}

namespace Conv
{

// enums

/// function types
enum T_enDataType
{
   dataTypeUnknown,  ///< unknown data type
   dataTypeVoid,     ///< void data type
   dataTypeInt,      ///< integer data type
   dataTypeString,   ///< string data type
};


// structs

/// imported item info
typedef struct
{
   /// type of the function/global memory location
   T_enDataType m_enDataType;

   /// name of imported item
   std::string m_strName;

} ImportedItem;


// classes

class CodeCallback
{
public:
   /// ctor
   CodeCallback() throw() {}

   /// prints "say" string
   virtual void Say(Uint16 index) = 0;

   /// shows menu, with possible answers
   virtual Uint16 BablMenu(const std::vector<Uint16>& uiAnswerStringIds) = 0;

   /// executes external function
   virtual Uint16 ExternalFunc(const char* pszFuncName, ConvStack& stack) = 0;
};


/// \brief Conversation code virtual machine
/// \details Virtual machine to run conversation code loaded from cnv.ark files.
/// It emulates a forth-like stack-based opcode language with intrinsic functions
/// (called imported functions). The code segment contains 16-bit opcodes that
/// are executed one by one using the step() method. It returns false when the
/// execution is stopped due to an error or when conversation has finished.
/// The class uses the CodeCallback to let the user respond to
/// higher-level actions in the virtual machine.
///
/// The conversation code first has to be loaded using
/// Import::ConvLoader, then Init() can be called. When exiting,
/// Done() should be called to write back conversation globals for the given
/// conversation.
class CodeVM
{
public:
   /// ctor
   CodeVM();
   /// dtor
   virtual ~CodeVM() {}

   /// inits virtual machine after filling code segment
   void Init(CodeCallback* pCodeCallback, const Underworld::ConvGlobals& cg);

   /// does a step in code; returns false when program has stopped
   bool Step();

   /// writes back conv globals
   void Done(Underworld::ConvGlobals& cg);

   /// replaces all @ placeholder in the given string
   void ReplacePlaceholder(std::string& str);

   // get functions

   /// returns code segment
   std::vector<Uint16>& CodeSegment(){ return m_code; }

   /// returns map with imported functions
   std::map<Uint16, ImportedItem>& ImportedFuncs(){ return m_mapImportedFuncs; }
   /// returns map with imported globals
   std::map<Uint16, ImportedItem>& ImportedGlobals(){ return  m_mapImportedGlobals; }

   /// returns string block to use for this conversation
   Uint16 StringBlock() const { return m_uiStringBlock; }

   /// returns number of reserved global variables
   Uint16 GlobalsReserved() const { return m_uiGlobalsReserved; }

   /// returns local string value
   virtual std::string GetLocalString(Uint16 uiStrNum);

   // set functions

   /// sets result register
   void SetResultRegister(Uint16 val);

   /// sets conversation slot
   void ConvSlot(Uint16 uiConvSlot){ m_uiConvSlot = uiConvSlot; }

   /// sets string block to use
   void StringBlock(Uint16 uiStringBlock){ m_uiStringBlock = uiStringBlock; }

   /// sets number of globals reserved at start of memory
   void GlobalsReserved(Uint16 uiGlobalsReserved){ m_uiGlobalsReserved = uiGlobalsReserved; }

protected:
   /// called when saying a string
   void SayOp(Uint16 uiStringId);

   /// executes an imported function
   virtual void ImportedFunc(const char* pszFuncName);

   /// queries for a global variable value
   virtual Uint16 GetGlobal(const char* pszGlobName);

   /// sers global variable value
   virtual void SetGlobal(const char* pszGlobName, Uint16 val);

   /// called when storing a value to the stack
   void StoreValue(Uint16 at, Uint16 val);

   /// called when fetching a value from stack
   void FetchValue(Uint16 at);

protected:
   /// conversation slot number
   Uint16 m_uiConvSlot;

   /// number of string block to use
   Uint16 m_uiStringBlock;

   /// code bytes
   std::vector<Uint16> m_code;

   /// instruction pointer
   Uint16 m_uiInstrPtr;

   /// base (frame) pointer
   Uint16 m_uiBasePtr;

   /// stack
   ConvStack m_stack;

   /// number of values for globals reserved on stack
   Uint16 m_uiGlobalsReserved;

   /// tracks call/ret level
   unsigned int m_uiCallLevel;

   /// result register for (imported) functions
   Uint16 m_uiResultRegister;

   /// indicates if conversation has finished
   bool m_bFinished;

   /// all imported functions
   std::map<Uint16, ImportedItem> m_mapImportedFuncs;

   /// names of all imported globals
   std::map<Uint16, ImportedItem> m_mapImportedGlobals;

   /// code callback pointer
   CodeCallback* m_pCodeCallback;
};

} // namespace Conv
