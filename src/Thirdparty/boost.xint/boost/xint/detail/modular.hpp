
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
    \brief Contains the modular math functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_MODULAR_HPP
#define BOOST_INCLUDED_XINT_MODULAR_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT mulmod(const BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT
    &by, const BOOST_XINT_RAWINT &modulus)
{
    return n * by % modulus;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT sqrmod(const BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT
    &modulus)
{
    return square(n) % modulus;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT powmod(const BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT &e,
    const BOOST_XINT_RAWINT &m, bool no_montgomery = false)
{
    if (m.is_zero() || m.negative) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_modulus());
    if (e.is_zero()) return 1;

    bool neg = (n.negative && e.is_odd());

    // Montgomery's method is often noticeably faster, but only works if the
    // m is odd.
    if (m.is_odd() && !no_montgomery) {
        return montgomeryPowerMod(n.abs() % m, e.abs(), m);
    } else {
        BOOST_XINT_RAWINT answer(1), p(n.abs());

        std::size_t lastBitCount = 0;
        detail::digit_t ee(e[e.length - 1]);
        while (ee != 0) { ee >>= 1; ++lastBitCount; }

        for (std::size_t eIndex = 0; eIndex < e.length; ++eIndex) {
            detail::digit_t ee(e[eIndex]);

            int bitCount(int(eIndex == e.length - 1 ? lastBitCount :
                detail::bits_per_digit));
            while (bitCount-- > 0) {
                if (ee & 0x01) answer = mulmod(answer, p, m);
                p = sqrmod(p, m);
                ee >>= 1;
            }
        }
        answer.negative = neg;
        answer.trim();

        return answer;
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_MODULAR_HPP
