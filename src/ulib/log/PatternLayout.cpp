//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2012 Michael Fink
//
/// \file PatternLayout.cpp pattern layout implementation
//
// includes
#include "stdafx.h"
#include <ulib/Log/Layout.hpp>

void Log::PatternLayout::Format(CString& cszOutput, const LoggingEventPtr spLoggingEvent) throw()
{
   ATLASSERT(spLoggingEvent.get() != NULL);

   cszOutput = m_cszPattern;

   int iPos = 0, iCurrent = 0, iMax = cszOutput.GetLength();
   while (iCurrent < iMax)
   {
      iCurrent = iPos = cszOutput.Find(_T('%'), iCurrent);
      if (iPos == -1)
         break; // no more format specifiers

      // read format specifier
      iPos++;
      if (iPos >= iMax)
         break; // finished in the middle of parsing

      TCHAR ch = cszOutput[iPos];

      bool bLeftJustify = false; // right justify is default

      // read possible left justify char
      if (ch == _T('-'))
      {
         bLeftJustify = true;
         iPos++;
         if (iPos >= iMax)
            break; // finished

         ch = cszOutput[iPos];
      }

      /// read min width when available
      int iMinWidth = -1;
      while (ch >= _T('0') && ch <= _T('9'))
      {
         if (iMinWidth < 0)
            iMinWidth = 0;
         else
            iMinWidth *= 10;
         iMinWidth += ch - _T('0');

         iPos++;
         if (iPos >= iMax)
            break; // finished

         ch = cszOutput[iPos];
      }

      if (iPos >= iMax)
         break; // finished

      // read point when available
      int iMaxWidth = -1;
      if (ch == _T('.'))
      {
         iPos++;
         if (iPos >= iMax)
            break; // finished

         ch = cszOutput[iPos];

         // now read max width
         while (ch >= _T('0') && ch <= _T('9'))
         {
            if (iMaxWidth < 0)
               iMaxWidth = 0;
            else
               iMaxWidth *= 10;
            iMaxWidth += ch - _T('0');

            iPos++;
            if (iPos >= iMax)
               break; // finished

            ch = cszOutput[iPos];
         }

         if (iPos >= iMax)
            break; // finished
      }

      // when we get here, ch contains the specification character
      iPos++; // set pos to beyond char

      // get string to replace with
      CString cszReplaceString;

      switch(ch)
      {
      case _T('c'): // outputs logger name
         cszReplaceString = spLoggingEvent->LoggerName();
         break;
      case _T('d'): // date in ISO 8601 format
         cszReplaceString = spLoggingEvent->Timestamp().FormatISO8601(DateTime::formatYMD_HMSF_Z, false);
         break;
      case _T('F'): // source filename where log message occured
         cszReplaceString = spLoggingEvent->SourceFilename();
         break;
      case _T('l'): // location
		  cszReplaceString = spLoggingEvent->SourceFilename();
         break;
      case _T('L'): // source file line where log message occured
         cszReplaceString.Format(_T("%u"), spLoggingEvent->SourceLine());
         break;
      case _T('m'): // log message
         cszReplaceString = spLoggingEvent->Message();
         break;
      case _T('n'): // platform-specific newline character
         cszReplaceString = _T("\n");
         break;
      case _T('p'): // log level (priority)
         switch(spLoggingEvent->Level())
         {
         case debug: cszReplaceString = _T("DEBUG"); break;
         case info:  cszReplaceString = _T("INFO"); break;
         case warn:  cszReplaceString = _T("WARN"); break;
         case error: cszReplaceString = _T("ERROR"); break;
         case fatal: cszReplaceString = _T("FATAL"); break;
         case none:  cszReplaceString = _T("NONE"); break;
         default:
            ATLASSERT(false);
            break;
         }
         break;
      case _T('r'): // relative time elapsed; not supported
         cszReplaceString = _T("");
         break;
      case _T('t'): // thread id
         cszReplaceString.Format(_T("%u"), spLoggingEvent->ThreadId());
         break;
      case _T('%'): // percent sign
         cszReplaceString = _T("%");
         break;
      default:
         ATLASSERT(false); // unknown format character
         break;
      }

      // now that we have the string to be replaced, apply min width, max width and left justification
      if (iMinWidth >= 0)
      {
         // check if we have to pad the string
         int iLen = cszReplaceString.GetLength();
         if (iLen < iMinWidth)
         {
            // yes, pad
            if (bLeftJustify)
               cszReplaceString += CString(_T(' '), iMinWidth-iLen); // on the right
            else
               cszReplaceString = CString(_T(' '), iMinWidth-iLen) + cszReplaceString; // on the left
         }
      }

      // now check if we have to truncate the string
      if (iMaxWidth >= 0)
      {
         int iLen = cszReplaceString.GetLength();

         // cut off from the beginning, just as log4cxx does, and different to the printf behavior
         if (iLen > iMaxWidth)
            cszReplaceString = cszReplaceString.Right(iMaxWidth);
      }

      // delete old string
      cszOutput.Delete(iCurrent, iPos-iCurrent);
      // insert new one
      cszOutput.Insert(iCurrent, cszReplaceString);

      iCurrent += cszReplaceString.GetLength();
      iMax = cszOutput.GetLength();
   }
}
