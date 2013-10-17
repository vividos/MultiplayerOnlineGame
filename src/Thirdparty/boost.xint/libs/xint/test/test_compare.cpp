
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Contains tests for the comparison functions.
*/

#include <boost/xint/integer.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

namespace boost {
namespace xint {

using namespace boost::xint::options;
typedef integer_t<fixedlength<8> > integer8;
typedef integer_t<fixedlength<12> > integer12;

BOOST_AUTO_TEST_CASE(test_compare_zero) {
    integer zero(0), one(1), negative_one(-1);
    BOOST_CHECK_EQUAL(zero, zero);
    BOOST_CHECK_LT(zero, one);
    BOOST_CHECK_GT(zero, negative_one);
    BOOST_CHECK_GT(one, zero);
    BOOST_CHECK_LT(negative_one, zero);
}

BOOST_AUTO_TEST_CASE(test_compare_negatives) {
    integer i1("-0x12345678901234567890", autobase),
        i2("-0x23456789012345678901", autobase);
    BOOST_CHECK_GT(i1, i2);
    BOOST_CHECK_LT(i2, i1);
}

BOOST_AUTO_TEST_CASE(test_compare_positives) {
    integer i1("0x12345678901234567890", autobase),
        i2("0x12345678901234567891", autobase),
        i3(1);
    BOOST_CHECK_LT(i1, i2);
    BOOST_CHECK_GT(i1, i3);
}

BOOST_AUTO_TEST_CASE(test_compare_mixed_signs) {
    integer i1("-0x12345678901234567890", autobase),
        i2("0x12345678901234567890", autobase);
    BOOST_CHECK_LT(i1, i2);
}

BOOST_AUTO_TEST_CASE(test_compare_fixed_overflow) {
    BOOST_CHECK_GT(integer8(255), 256);
    BOOST_CHECK_LT(integer8(-255), -256);
    BOOST_CHECK_LT(256, integer8(255));
    BOOST_CHECK_GT(-256, integer8(-255));
}

BOOST_AUTO_TEST_CASE(test_compare_fixed_different_sizes) {
    integer12 i1(0x890);
    integer8 i2(0xA0);
    BOOST_CHECK_GT(i1, integer12(i2));
}

} // namespace xint
} // namespace boost

