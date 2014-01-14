//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file ReaderWriterMutex.hpp reader/writer mutex
//
#pragma once

// includes
#include <ulib/config/AutoLink.hpp>
#include <boost/noncopyable.hpp>
#include <memory>

/// \brief reader-writer mutex
class ULIB_DECLSPEC ReaderWriterMutex: public boost::noncopyable
{
public:
   /// ctor
   ReaderWriterMutex();
   /// dtor
   ~ReaderWriterMutex();

private:
   friend class ReaderLock;
   friend class WriterLock;

   class Impl;
   /// implementation pointer
   std::shared_ptr<Impl> m_spImpl;
};

/// \brief reader lock
/// \details multiple reader locks are possible
class ULIB_DECLSPEC ReaderLock: public boost::noncopyable
{
public:
   /// ctor; locks the mutex as reader
   ReaderLock(ReaderWriterMutex& mutex);
   /// dtor; releases the mutex
   ~ReaderLock();

private:
   /// ref to mutex
   ReaderWriterMutex& m_mutex;
};

/// \brief writer lock
/// \details only one writer lock is possible
class ULIB_DECLSPEC WriterLock: public boost::noncopyable
{
public:
   /// ctor; locks the mutex as writer
   WriterLock(ReaderWriterMutex& mutex);
   /// dtor; releases the mutex
   ~WriterLock();

private:
   /// ref to mutex
   ReaderWriterMutex& m_mutex;
};
