//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SharedBuffer.hpp Shared buffer for Boost.Asio
//
#pragma once

// includes
#include "Asio.hpp"
#include <vector>

/// a reference-counted non-modifiable buffer class
/// from boost::asio examples; see:
/// http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/buffers/reference_counted.cpp
class SharedConstBuffer
{
public:
   /// construct from a given vector
   template <typename InputIterator>
   explicit SharedConstBuffer(InputIterator itBegin, InputIterator itEnd)
      :m_spData(new std::vector<unsigned char>(itBegin, itEnd)),
       m_buffer(boost::asio::buffer(*m_spData))
   {
   }

   /// returns data
   std::vector<unsigned char>& Data(){ return *m_spData; }

   // implement the ConstBufferSequence requirements
   typedef boost::asio::const_buffer value_type;                           ///< buffer value type
   typedef const boost::asio::const_buffer* const_iterator;                ///< const interator type
   const boost::asio::const_buffer* begin() const { return &m_buffer; }    ///< buffer begin() method
   const boost::asio::const_buffer* end() const { return &m_buffer + 1; }  ///< buffer end() method

private:
   /// shared data
   std::shared_ptr<std::vector<unsigned char>> m_spData;

   /// const buffer definition
   boost::asio::const_buffer m_buffer;
};

/// reference-count modifiable buffer class
class SharedMutableBuffer
{
public:
   /// ctor
   SharedMutableBuffer(size_t uiSize)
      :m_spData(new std::vector<unsigned char>(uiSize)),
       m_buffer(boost::asio::buffer(*m_spData))
   {
   }

   /// returns data
   std::vector<unsigned char>& Data(){ return *m_spData; }

   // implement the MutableBufferSequence requirements
   typedef boost::asio::mutable_buffer value_type;                            ///< buffer value type
   typedef const boost::asio::mutable_buffer* const_iterator;                 ///< const interator type
   const boost::asio::mutable_buffer* begin() const { return &m_buffer; }     ///< buffer begin() method
   const boost::asio::mutable_buffer* end() const { return &m_buffer + 1; }   ///< buffer end() method

private:
   /// shared data
   std::shared_ptr<std::vector<unsigned char>> m_spData;

   /// mutable buffer definition
   boost::asio::mutable_buffer m_buffer;
};
