//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/StringsPakFile.cpp Strings.pak file loader
//

// includes
#include "stdafx.h"
#include "StringsPakFile.hpp"
#include "GameStrings.hpp"
#include <ulib/stream/EndianAwareFilter.hpp>

using Import::StringsPakFile;

/// define this when you can be sure the strings.pak is correct;
/// speeds up loading a bit
#define HAVE_CORRECT_STRINGS_FILE

// StringsPakFile methods

StringsPakFile::StringsPakFile(std::shared_ptr<Stream::IStream> spStream)
:m_spStream(spStream)
{
   LoadNodeList();
}

void StringsPakFile::LoadNodeList()
{
   Stream::EndianAwareFilter filter(*m_spStream);

   // number of huffman nodes
   Uint16 nodenum = filter.Read16LE();

   // read in node list
   allnodes.resize(nodenum);
   for (Uint16 k=0; k<nodenum; k++)
   {
      allnodes[k].symbol = m_spStream->ReadByte();
      allnodes[k].parent = m_spStream->ReadByte();
      allnodes[k].left   = m_spStream->ReadByte();
      allnodes[k].right  = m_spStream->ReadByte();
   }

   // number of string blocks
   Uint16 sblocks = filter.Read16LE();

   // read in all block infos
   for (int z=0; z<sblocks; z++)
   {
      Uint16 uiBlockId = filter.Read16LE();
      Uint32 offset = filter.Read32LE();

      m_mapAllBlocks.insert(std::make_pair(uiBlockId, offset));
   }
}

void StringsPakFile::LoadAllBlocks(GameStrings& strings)
{
   for (auto iter = m_mapAllBlocks.begin(), stop = m_mapAllBlocks.end(); iter != stop; iter++)
   {
      Uint16 uiBlockId = iter->first;

      strings.m_mapAllStrings[uiBlockId] = std::vector<std::string>();

      std::vector<std::string>& vecStringBlock = strings.m_mapAllStrings[uiBlockId];

      LoadStringBlock(uiBlockId, vecStringBlock);
   };
}

/// Loads a single string block from file. Please check with IsAvail() if the
/// block is available.
/// \param uiBlockId block id of string block to load
/// \param vecStringBlock strings array that gets filled
void StringsPakFile::LoadStringBlock(Uint16 uiBlockId, std::vector<std::string>& vecStringBlock)
{
   auto iter = m_mapAllBlocks.find(uiBlockId);
   if (iter == m_mapAllBlocks.end())
   {
      UaTrace("loading string block %04x failed\n", uiBlockId);
      return;
   }

   Uint32 offset = m_mapAllBlocks[uiBlockId];

   // determine filesize
#ifndef HAVE_CORRECT_STRINGS_FILE
   ULONGLONG ullFileSize = m_spStream->Length();
#endif

   m_spStream->Seek(offset, Stream::IStream::seekBegin);

   Stream::EndianAwareFilter filter(*m_spStream);

   // number of strings
   Uint16 numstrings = filter.Read16LE();

   // all string offsets
   std::vector<Uint16> stroffsets;
   stroffsets.resize(numstrings);

   for (int j=0; j<numstrings; j++)
      stroffsets[j] = filter.Read16LE();

   Uint32 curoffset = offset + (numstrings+1)*sizeof(Uint16);
   unsigned int nodenum = allnodes.size();

   for (Uint16 n=0; n<numstrings; n++)
   {
      m_spStream->Seek(curoffset+stroffsets[n], Stream::IStream::seekBegin);

      char c;
      std::string str;

      int bit = 0;
      int raw = 0;

      do
      {
         int node=nodenum-1; // starting node

         // huffman tree decode loop
         while (char(allnodes[node].left) != -1 && char(allnodes[node].left) != -1)
         {
            if (bit==0)
            {
               bit=8;
               raw = m_spStream->ReadByte();

#ifndef HAVE_CORRECT_STRINGS_FILE
               if (m_spStream->Position() >= ullFileSize)
               {
                  // premature end of file, should not happen
                  n = numstrings;
                  //uiBlockId=sblocks;
                  break;
               }
#endif
            }

            // decide which node is next
            node = (raw & 0x80) ? short(allnodes[node].right)
               : short(allnodes[node].left);

            raw<<=1;
            bit--;
         }

#ifndef HAVE_CORRECT_STRINGS_FILE
         if (m_spStream->Position() >= ullFileSize)
            break;
#endif

         // have a new symbol
         c = static_cast<char>(allnodes[node].symbol);
         if (c!='|')
            str.append(1,c);

      } while (c!='|');

      vecStringBlock.push_back(str);
      str.erase();
   }

   // remove excess memory
   std::vector<std::string>(vecStringBlock).swap(vecStringBlock);
}
