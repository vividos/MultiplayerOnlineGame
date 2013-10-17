
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the test functions for the Montgomery stuff.
*/

#include <boost/xint/integer.hpp>
#include <boost/xint/random.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>

namespace boost {
namespace xint {

using std::endl;

namespace {

void _test1(size_t section, size_t test, integer x, integer y, integer m) {
    integer xx, yy, mResult;
    toMontgomeryForm(xx._data(), x._data(), m._data());
    toMontgomeryForm(yy._data(), y._data(), m._data());
    fromMontgomeryForm(mResult._data(), montgomeryMultiplyMod(xx._data(),
        yy._data(), m._data(), inverse0(m._data())), m._data());

    integer cResult = x * y % m;

    if (mResult != cResult && (-mResult % m) != cResult) {
        std::ostringstream out;
        out << "testMontyMultiply, section " << section << ", " << test << endl;
        out << "x        = " << to_string(x, 16) << endl;
        out << "y        = " << to_string(y, 16) << endl;
        out << "m        = " << to_string(m, 16) << endl;
        out << "answer   = " << to_string(mResult, 16) << endl;
        out << "expected = " << to_string(cResult, 16) << endl << endl;
        BOOST_ERROR(out.str());
    }
}

void _test2(size_t section, size_t test, integer b, integer r, integer modulus) {
    integer mResult(montgomeryPowerMod((b % modulus)._data(), r._data(),
        modulus._data()));
    integer cResult = powmod(b % modulus, r, modulus, true);
    if (mResult != cResult) {
        std::ostringstream out;
        out << "testMontyPowerMod, section " << section << ", " << test << endl;
        out << "b        = " << to_string(b, 16) << endl;
        out << "r        = " << to_string(r, 16) << endl;
        out << "m        = " << to_string(modulus, 16) << endl;
        out << "answer   = " << to_string(mResult, 16) << endl;
        out << "expected = " << to_string(cResult, 16) << endl << endl;
        BOOST_ERROR(out.str());
    }
}

} // namespace

BOOST_AUTO_TEST_CASE(test_monty_multiply) {
    // We always want these to run on the same data, so we can reproduce it if
    // there are problems.
    boost::mt19937 random(42u);

    {
        integer n("ffffffffaec73757", 16);
        integer m("-3856431e", 16);
        integer modulus("10000000000000000fffffffff14e691d", 16);
        _test1(0, 1, n, m, modulus);
    }

    for (size_t nsize = detail::bits_per_digit * 2; nsize <
        detail::bits_per_digit * 4; nsize += 4)
    {
        for (size_t msize = detail::bits_per_digit * 2; msize <
            detail::bits_per_digit * 4; msize += 4)
        {
            for (size_t modsize = detail::bits_per_digit * 4; msize <
                detail::bits_per_digit * 5; msize += 4)
            {
                integer n(integer::random_by_size(random, nsize, false, false,
                    true)), m(integer::random_by_size(random, msize, false,
                    false, true)), modulus(integer::random_by_size(random,
                    modsize, true, true, false));
                _test1(nsize, msize, n, m, modulus);
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(test_monty_powmod) {
    // We always want these to run on the same data, so we can reproduce it if
    // there are problems.
    boost::mt19937 random(42u);

    {
        integer modulus(integer::random_by_size(random, detail::bits_per_digit *
            5 + (detail::bits_per_digit / 2), true, true));
        for (int i=2; i<100; ++i)
            _test2(0, i, integer("abcd1234", 16), i, modulus);
    }

    for (size_t nsize = detail::bits_per_digit * 2; nsize <
        detail::bits_per_digit * 4; nsize += 8)
    {
        for (size_t msize = detail::bits_per_digit * 2; msize <
            detail::bits_per_digit * 4; msize += 8)
        {
            for (size_t modsize = detail::bits_per_digit * 2; msize <
                detail::bits_per_digit * 5; msize += 8)
            {
                integer n(integer::random_by_size(random, nsize, false, false)),
                    m(integer::random_by_size(random, msize, false, false)),
                    modulus(integer::random_by_size(random, modsize, true,
                    true));
                _test2(nsize, msize, n, m, modulus);
            }
        }
    }
}

} // namespace xint
} // namespace boost
