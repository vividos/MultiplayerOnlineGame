//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SharedBuffer.hpp Shared buffer for Boost.Asio
//
#pragma once

// includes
#include "Asio.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

/// a reference-counted non-modifiable buffer class
/// from boost::asio examples; see:
/// http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/buffers/reference_counted.cpp
class SharedConstBuffer
{
public:
   // construct from a given vector
   template <typename InputIterator>
   explicit SharedConstBuffer(InputIterator itBegin, InputIterator itEnd)
      :m_spData(new std::vector<unsigned char>(itBegin, itEnd)),
       m_buffer(boost::asio::buffer(*m_spData))
   {
   }

   std::vector<unsigned char>& Data(){ return *m_spData; }

   // implement the ConstBufferSequence requirements
   typedef boost::asio::const_buffer value_type;
   typedef const boost::asio::const_buffer* const_iterator;
   const boost::asio::const_buffer* begin() const { return &m_buffer; }
   const boost::asio::const_buffer* end() const { return &m_buffer + 1; }

private:
   boost::shared_ptr<std::vector<unsigned char> > m_spData;
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
   typedef boost::asio::mutable_buffer value_type;
   typedef const boost::asio::mutable_buffer* const_iterator;
   const boost::asio::mutable_buffer* begin() const { return &m_buffer; }
   const boost::asio::mutable_buffer* end() const { return &m_buffer + 1; }

private:
   /// shared data
   boost::shared_ptr<std::vector<unsigned char> > m_spData;

   /// buffer
   boost::asio::mutable_buffer m_buffer;
};
