
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
    \brief Contains the square-root function definition, and any future
           root-related ones.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_ROOTS_HPP
#define BOOST_INCLUDED_XINT_ROOTS_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT square_root(const BOOST_XINT_RAWINT nn) {
    if (nn.is_zero()) return 0;
    if (nn.negative) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::cannot_represent("cannot represent imaginary values (tried "
        "to take square_root of negative number)"));

    // A naive implementation using pure integers can result in an endless loop,
    // cycling between two numbers that are approximately correct (try
    // sqrt(23)). To deal with that, we multiply everything by an even power of
    // two.
    const std::size_t extra_bits = 1;
    BOOST_XINT_RAWINT n(nn << (extra_bits * 2));

    // Initial guess is half the length of n, in bits
    BOOST_XINT_RAWINT target;
    setbit(target, log2(n) / 2);

    // Now refine it until we're as close as we can get.
    while (1) {
        BOOST_XINT_RAWINT guess((target + (n / target)) >> 1);
        if ((target >> extra_bits) == (guess >> extra_bits)) break;
        target = guess;
    }

    // Remove half of the added bits.
    target >>= extra_bits;
    return target;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_ROOTS_HPP
