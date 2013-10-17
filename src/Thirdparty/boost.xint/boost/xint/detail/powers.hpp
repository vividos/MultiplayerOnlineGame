
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
    \brief Contains functions related to powers of a number.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_POWERS_HPP
#define BOOST_INCLUDED_XINT_POWERS_HPP

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

BOOST_XINT_RAWINT_TPL
void pow(BOOST_XINT_RAWINT &target, const BOOST_XINT_RAWINT &n, const
    BOOST_XINT_RAWINT &e)
{
    if (e.is_zero()) {
        // Anything to the zeroth power is one.
        target.set(1);
        return;
    } else if (e.negative) {
        // Anything to a negative power is a fractional value, which an integer
        // library can't represent.
        target.set(0);
        return;
    }

    bool neg = (!n.is_zero() && n.negative && e.is_odd());

    std::size_t lastBitCount = 0;
    digit_t ee(e[e.length - 1]);
    while (ee != 0) { ee >>= 1; ++lastBitCount; }

    BOOST_XINT_RAWINT p(n.abs());
    target.set(1);
    for (std::size_t eIndex = 0; eIndex < e.length; ++eIndex) {
        digit_t eee(e[eIndex]);

        int bitCount(int(eIndex == e.length - 1 ? lastBitCount :
            bits_per_digit));
        while (bitCount-- > 0) {
            if (eee & 0x01) target *= p;
            p = square(p);
            eee >>= 1;
        }
    }
    target.negative = neg;
    target.trim();
}

BOOST_XINT_RAWINT_TPL void pow2(BOOST_XINT_RAWINT &target, std::size_t exponent)
{
    target.set(0);
    setbit(target, exponent);
    target.trim();
}

BOOST_XINT_RAWINT_TPL void factorial(BOOST_XINT_RAWINT &target, std::size_t n) {
    target.set(1);

    BOOST_XINT_RAWINT one(1), nn(n);
    while (nn > one) {
        target *= nn;
        --nn;
    }
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_POWERS_HPP
