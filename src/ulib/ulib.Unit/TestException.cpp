//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012 Michael Fink
//
/// \file TestException.cpp tests for Exception class
//

// includes
#include "stdafx.h"
#include <ulib/Exception.hpp>
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests Exception class
TEST_CLASS(TestException)
{
public:
   /// tests throwing and catching by ...
   TEST_METHOD(TestException1)
   {
      try
      {
         throw Exception(__FILE__, __LINE__);
      }
      catch(...)
      {
         return;
      }
   }

   /// tests if Exception is derived from std::exception (it shouldn't be)
   TEST_METHOD(TestException2)
   {
      try
      {
         throw Exception(__FILE__, __LINE__);
      }
      catch(std::exception& e)
      {
         e;
   #pragma warning(push)
   #pragma warning(disable: 4127) // conditional expression is constant
         Assert::IsTrue(false);
   #pragma warning(pop)
      }
      catch(Exception& e)
      {
         e;
      }
   }

   /// tests rethrowing exception
   TEST_METHOD(TestException3)
   {
      try
      {
         try
         {
            throw Exception(__FILE__, __LINE__);
         }
         catch(Exception& e)
         {
            throw e;
         }
      }
      catch(Exception& e)
      {
         e;
         return;
      }
   }
};

} // namespace UnitTest
