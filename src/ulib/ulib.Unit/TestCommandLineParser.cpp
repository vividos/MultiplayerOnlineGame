//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012 Michael Fink
//
/// \file TestCommandLineParser.cpp tests for CommandLineParser class
//

// includes
#include "stdafx.h"
#include <ulib/CommandLineParser.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

TEST_CLASS(TestCommandLineParser)
{
public:
   /// tests CommandLineParser, empty cmdline
   TEST_METHOD(TestParser1)
   {
      CommandLineParser clp(_T(""));

      CString c1;
      Assert::IsTrue(false == clp.GetNext(c1));
   }

   /// tests CommandLineParser, some words
   TEST_METHOD(TestParser2)
   {
      CommandLineParser clp(_T("abc def 123   xyz  a"));

      CString c1;
      Assert::IsTrue(true == clp.GetNext(c1)); Assert::IsTrue(c1 == _T("abc"));
      Assert::IsTrue(true == clp.GetNext(c1)); Assert::IsTrue(c1 == _T("def"));
      Assert::IsTrue(true == clp.GetNext(c1)); Assert::IsTrue(c1 == _T("123"));
      Assert::IsTrue(true == clp.GetNext(c1)); Assert::IsTrue(c1 == _T("xyz"));
      Assert::IsTrue(true == clp.GetNext(c1)); Assert::IsTrue(c1 == _T("a"));
      Assert::IsTrue(false == clp.GetNext(c1));
   }

   /// parses command line with argc and argv values
   TEST_METHOD(TestParserArgcArgv)
   {
      TCHAR* argv[] =
      {
         _T("arg1"),
         _T("\"arg2"),
         _T("arg2\""),
      };

      int argc = sizeof(argv)/sizeof(*argv);
      CommandLineParser clp(argc, argv);

      CString c1;
      Assert::IsTrue(true == clp.GetNext(c1));
      Assert::IsTrue(c1 == argv[0]);

      Assert::IsTrue(true == clp.GetNext(c1));
      Assert::IsTrue(c1 == _T("arg2 arg2"));

      Assert::IsTrue(false == clp.GetNext(c1));
   }

   TEST_METHOD(TestParseParams)
   {
      CommandLineParser clp(_T("abc 123 /test qwerty --linux-help"));

      CString cszParam;
      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("abc") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("123") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("/test") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("qwerty") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("--linux-help") == cszParam);

      Assert::IsTrue(false == clp.GetNext(cszParam));
   }

   TEST_METHOD(TestParseQuotedFilenames)
   {
      CommandLineParser clp(_T("\"some\" \"quoted\" text \"in cmdline\" "));

      CString cszParam;
      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("some") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("quoted") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("text") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("in cmdline") == cszParam);

      Assert::IsTrue(false == clp.GetNext(cszParam));
   }

   TEST_METHOD(TestParseQuotesInString)
   {
      CommandLineParser clp(_T("\"ab cd\" def \"asdf"));

      CString cszParam;
      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("ab cd") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("def") == cszParam);

      Assert::IsTrue(true == clp.GetNext(cszParam));
      Assert::IsTrue(_T("asdf") == cszParam);

      Assert::IsTrue(false == clp.GetNext(cszParam));
   }
};

} // namespace UnitTest
