
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the division testing functions. It also tests some other
    operations, such as multiplication, but only indirectly and not very
    thoroughly.
*/

#include <boost/xint/integer.hpp>
#include <boost/xint/random.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>

namespace boost {
namespace xint {

using std::endl;

namespace {

void _test(size_t section, size_t test, integer n, integer m) {
    integer::divide_t a(divide(n, m));
    integer answer(a.quotient), remainder(a.remainder);

    integer check=answer*m+remainder;
    if (check != n || abs(remainder) >= abs(m)) {
        bool flagRemainder=(check == n);

        std::ostringstream out;
        out << "testDivideMod, section div-" << section << ", " << test << endl;
        out << "n         = " << to_string(n, 16) << endl;
        out << "m         = " << to_string(m, 16) << endl;
        out << "a         = " << to_string(answer, 16) << endl;
        out << "r         = " << to_string(remainder, 16)
            << (flagRemainder ? " (should be < m, isn't)" : "")
            << endl << endl;

        out << "a * m + r = " << to_string(check, 16)
            << (check != n ? " (should be n, isn't)" : " (ok)")
            << endl;
        BOOST_ERROR(out.str());
    }

    integer modAnswer = n % m;
    bool modValid=(modAnswer == remainder || modAnswer == remainder + abs(m));
    if (modAnswer >= abs(m) || modAnswer < 0 || !modValid) {
        std::ostringstream out;
        out << "testDivideMod, section mod-" << section << ", " << test << endl;
        out << "n         = " << to_string(n, 16) << endl;
        out << "m         = " << to_string(m, 16) << endl;
        out << "a         = " << to_string(modAnswer, 16) << endl;

        out << "expected  = " << to_string(remainder, 16) << endl;
        out << "or        = " << to_string(remainder+abs(m), 16) << endl;
        BOOST_ERROR(out.str());
    }
}

} // namespace

BOOST_AUTO_TEST_CASE(test_divide) {
    boost::mt19937 random(42u);

    for (size_t nsize = detail::bits_per_digit * 3; nsize <
        detail::bits_per_digit * 5; nsize += 4)
    {
        for (size_t msize = detail::bits_per_digit * 3; msize <
            detail::bits_per_digit * 5; msize += 4)
        {
            integer n, m;
            n = integer::random_by_size(random, nsize, false, false, true);
            m = integer::random_by_size(random, msize, false, false, true);
            _test(nsize, msize, n, m);
        }
    }
}

} // namespace xint
} // namespace boost
