//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/StringsPakFile.hpp Strings.pak file loader
//
#pragma once

// includes

// forward references
class GameStrings;

namespace Stream
{
class IStream;
}

namespace Import
{

/// \brief strings.pak file class
/// note that uw1 and uw2 use the same format
/// also note that files created with the strpak tool can be used
class StringsPakFile
{
public:
   /// ctor; opens .pak file
   StringsPakFile(std::shared_ptr<Stream::IStream> spStream);

   /// loads all string blocks
   void LoadAllBlocks(GameStrings& strings);

private:
   /// loads node list
   void LoadNodeList();

   /// loads a single string block
   void LoadStringBlock(Uint16 uiBlockId, std::vector<std::string>& vecStringBlock);

private:
   /// strings.pak stream
   std::shared_ptr<Stream::IStream> m_spStream;

   /// strings.pak huffman node structure
   struct HuffNode
   {
     int symbol; //!< character symbol in that node
     int parent; //!< parent node
     int left;   //!< left node (-1 when no node)
     int right;  //!< right node
   };

   /// a vector with all huffman nodes for the given .pak file
   std::vector<HuffNode> allnodes;

   /// a map of all blocks available in the file to offsets in the file
   std::map<Uint16, Uint32> m_mapAllBlocks;
};

} // namespace Import
