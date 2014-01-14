//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2014 Michael Fink
//
/// \file SRPPresets.cpp Presets for SRP group parameters
//

// includes
#include "stdafx.h"
#include "SRPHelper.hpp"

using namespace SRP;

namespace SRP
{

/// stores preset SRP group parameters
struct PresetGroupParameter
{
   unsigned int uiGenerator;  ///< generator value
   LPCSTR pszaPrime;          ///< prime, as hex digits
   unsigned int uiNumBits;    ///< number of bits in prime
};

} // namespace SRP

/// SRP Group Parameters from RFC 5054 Appendix A
PresetGroupParameter g_aGroupParameter [] =
{
   // 1024-bit Group
   { 2, "EEAF0AB9 ADB38DD6 9C33F80A FA8FC5E8 60726187 75FF3C0B 9EA2314C"
        "9C256576 D674DF74 96EA81D3 383B4813 D692C6E0 E0D5D8E2 50B98BE4"
        "8E495C1D 6089DAD1 5DC7D7B4 6154D6B6 CE8EF4AD 69B15D49 82559B29"
        "7BCF1885 C529F566 660E57EC 68EDBC3C 05726CC0 2FD4CBF4 976EAA9A"
        "FD5138FE 8376435B 9FC61D2F C0EB06E3",
     1024
   },

   // 1536-bit Group
   { 2, "9DEF3CAF B939277A B1F12A86 17A47BBB DBA51DF4 99AC4C80 BEEEA961"
        "4B19CC4D 5F4F5F55 6E27CBDE 51C6A94B E4607A29 1558903B A0D0F843"
        "80B655BB 9A22E8DC DF028A7C EC67F0D0 8134B1C8 B9798914 9B609E0B"
        "E3BAB63D 47548381 DBC5B1FC 764E3F4B 53DD9DA1 158BFD3E 2B9C8CF5"
        "6EDF0195 39349627 DB2FD53D 24B7C486 65772E43 7D6C7F8C E442734A"
        "F7CCB7AE 837C264A E3A9BEB8 7F8A2FE9 B8B5292E 5A021FFF 5E91479E"
        "8CE7A28C 2442C6F3 15180F93 499A234D CF76E3FE D135F9BB",
     1536
   },

   // 2048-bit Group
   { 2, "AC6BDB41 324A9A9B F166DE5E 1389582F AF72B665 1987EE07 FC319294"
        "3DB56050 A37329CB B4A099ED 8193E075 7767A13D D52312AB 4B03310D"
        "CD7F48A9 DA04FD50 E8083969 EDB767B0 CF609517 9A163AB3 661A05FB"
        "D5FAAAE8 2918A996 2F0B93B8 55F97993 EC975EEA A80D740A DBF4FF74"
        "7359D041 D5C33EA7 1D281E44 6B14773B CA97B43A 23FB8016 76BD207A"
        "436C6481 F1D2B907 8717461A 5B9D32E6 88F87748 544523B5 24B0D57D"
        "5EA77A27 75D2ECFA 032CFBDB F52FB378 61602790 04E57AE6 AF874E73"
        "03CE5329 9CCC041C 7BC308D8 2A5698F3 A8D0C382 71AE35F8 E9DBFBB6"
        "94B5C803 D89F7AE4 35DE236D 525F5475 9B65E372 FCD68EF2 0FA7111F"
        "9E4AFF73",
     2048
   },
};

unsigned int Helper::GetMaxCountPresetGroupParameter() throw()
{
   return sizeof(g_aGroupParameter)/sizeof(*g_aGroupParameter);
}

GroupParameter Helper::GetPresetGroupParameter(unsigned int uiIndex)
{
   if (uiIndex >= GetMaxCountPresetGroupParameter())
      throw std::runtime_error("invalid modulus/generator index");

   const PresetGroupParameter& preset = g_aGroupParameter[uiIndex];

   GroupParameter gp;
   gp.g = preset.uiGenerator;
   gp.uiNumBits = preset.uiNumBits;

   CStringA cszaPrime(preset.pszaPrime);
   cszaPrime.Remove(' ');

   gp.N = BigInteger((LPCSTR)cszaPrime, 16);

   //ATLASSERT(boost::xint::is_prime(gp.g) == 1); // 1 means: possible prime
   ATLASSERT(boost::xint::is_probably_prime(gp.g) == 1); // 1 means: possible prime

   return gp;
}
