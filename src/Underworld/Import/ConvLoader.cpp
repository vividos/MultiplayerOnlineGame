//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Import/ConvLoader.cpp Conversation code and conv. globals loader
//

// includes
#include "stdafx.h"
#include "ConvLoader.hpp"
#include "Bits.hpp"
#include "CodeVM.hpp"
#include "IFileSystem.hpp"
#include <ulib/stream/EndianAwareFilter.hpp>

/// \brief Loads conversation globals from bglobals.dat or babglobs.dat
/// When initial is set to true, the file to load only contains size entries,
/// and no actual data; all globals are initalized with 0 then.
void Import::ConvLoader::LoadConvGlobals(IFileSystem& fileSystem, Underworld::ConvGlobals& cg, bool bInitial)
{
   std::shared_ptr<Stream::IStream> spFile =
      fileSystem.OpenFile(bInitial ? _T("data\\babglobs.dat") :  _T("data\\bglobals.dat"), true);

   std::vector<std::vector<Uint16>>& allglobals = cg.GetVectorGlobalsList();

   Stream::EndianAwareFilter filter(*spFile);

   // read in all slot/size/[globals] infos
   while (!spFile->AtEndOfStream())
   {
      Uint16 slot = filter.Read16LE();
      Uint16 size = filter.Read16LE();

      std::vector<Uint16> globals;
      globals.resize(size,0);

      if (!bInitial)
      {
         // read in globals
         for(Uint16 i=0; i<size; i++)
            globals.push_back(filter.Read16LE());
      }

      // put globals in allglobals list
      if (slot >= allglobals.size())
         allglobals.resize(slot + 1);

      allglobals[slot] = globals;
   }
}

/// Loads conversation code from given file, into the virtual machine.
/// It loads all imported symbols (functions and globals) and puts the opcodes
/// into the vm's code segment. The method ua_conv_code_vm::init() can then be
/// called after this method.
/// \param fileSystem file system
/// \param vm conversation code virtual machine
/// \param conv convesation slot of code to load
bool Import::ConvLoader::LoadConvCode(IFileSystem& fileSystem, Conv::CodeVM& vm, Uint16 conv)
{
   std::shared_ptr<Stream::IStream> spFile =
      fileSystem.OpenFile(_T("data\\cnv.ark"), true);

   Stream::EndianAwareFilter filter(*spFile);

   // read number of entries
   Uint16 entries = filter.Read16LE();

   if (conv>=entries)
      throw Exception(_T("invalid conversation!"), __FILE__, __LINE__);

   spFile->Seek(conv * 4 + 2, Stream::IStream::seekBegin);

   Uint32 offset = filter.Read32LE();

   if (offset==0)
   {
      // no conversation available
      return false;
   }

   // read conversation header
   spFile->Seek(offset, Stream::IStream::seekBegin);

   Uint32 unk1 = filter.Read32LE(); // always 0x00000828
   ATLVERIFY(unk1 == 0x00000828);

   Uint16 codesize = filter.Read16LE();
   Uint16 unk2 = filter.Read16LE(); // always 0x0000
   Uint16 unk3 = filter.Read16LE(); // always 0x0000
   ATLVERIFY(unk2 == 0);
   ATLVERIFY(unk3 == 0);

   Uint16 strblock = filter.Read16LE(); // string block to use
   vm.StringBlock(strblock);

   Uint16 m_uiGlobalsReserved = filter.Read16LE(); // number of stack words reserved for globals
   vm.GlobalsReserved(m_uiGlobalsReserved);

   // load imported functions
   LoadImportedFuncs(vm, spFile);

   // load code
   std::vector<Uint16>& code = vm.CodeSegment();

   code.resize(codesize,0);
   for(Uint16 i=0; i<codesize; i++)
      code[i] = filter.Read16LE();

   // fix for Marrowsuck conversation; it has a wrong opcode on 0x076e
   if (conv == 6)
   {
      code[0x076e] = 0x0016; //op_PUSHI;
   }

   // fix for Sseetharee conversation; call to function at 0xffff
   if (conv == 15)
   {
      // just take another function that may seem reasonable
      code[0x0584] = 0x0666;
   }

   // fix for Judy conversation; argument to random is 2, but switch statement
   // checks for 3 values; the 3rd answer will never be given
   if (conv == 23)
   {
      // PUSHI #3
      code[0x04fd] = 3;
   }

   vm.ConvSlot(conv);

   return true;
}

void Import::ConvLoader::LoadImportedFuncs(Conv::CodeVM& vm, std::shared_ptr<Stream::IStream> spFile)
{
   std::map<Uint16, Conv::ImportedItem>& m_mapImportedFuncs = vm.ImportedFuncs();
   std::map<Uint16, Conv::ImportedItem>& m_mapImportedGlobals = vm.ImportedGlobals();

   m_mapImportedFuncs.clear();
   m_mapImportedGlobals.clear();

   Stream::EndianAwareFilter filter(*spFile);

   // read number of imported funcs
   Uint16 funcs = filter.Read16LE();

   for(int i=0; i<funcs; i++)
   {
      // length of function name
      Uint16 fname_len = filter.Read16LE();
      if (fname_len>255) fname_len = 255;

      char funcname[256];
      DWORD dwBytesRead = 0;
      spFile->Read(funcname, fname_len, dwBytesRead);
      funcname[fname_len] = 0;

      // function ID
      Uint16 func_id = filter.Read16LE();
      Uint16 fn_unknown1 = filter.Read16LE(); // always seems to be 1
      Uint16 import_type = filter.Read16LE();
      Uint16 ret_type = filter.Read16LE();
      ATLVERIFY(fn_unknown1 == 1);

      // fill imported item struct
      Conv::ImportedItem iitem;

      // determine return type
      switch (ret_type)
      {
      case 0x0000: iitem.m_enDataType = Conv::dataTypeVoid; break;
      case 0x0129: iitem.m_enDataType = Conv::dataTypeInt; break;
      case 0x012b: iitem.m_enDataType = Conv::dataTypeString; break;
      default:
         throw Exception(_T("unknown return type in conv imports list"), __FILE__, __LINE__);
      }

      iitem.m_strName = funcname;

      // store imported item in appropriate list
      switch (import_type)
      {
      case 0x0111:
         // imported function
         m_mapImportedFuncs[func_id] = iitem;
         break;

      case 0x010F:
         // imported global
         m_mapImportedGlobals[func_id] = iitem;
         break;

      default:
         throw Exception(_T("unknown import type in conv imports list"), __FILE__, __LINE__);
         break;
      }
   }
}
