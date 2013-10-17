
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
    \brief Contains functions using the Greatest Common Denominator algorithm.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_GCD_HPP
#define BOOST_INCLUDED_XINT_GCD_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
struct gcd_core {
    gcd_core(BOOST_XINT_RAWINT n, BOOST_XINT_RAWINT m): u1(1), u2(), u3(n) {
        const BOOST_XINT_RAWINT zero;
        BOOST_XINT_RAWINT t1(m), t2(n), t3(m);
        --t2;
        do {
            do {
                if (u3.is_even()) {
                    if (u1.is_odd() || u2.is_odd()) { u1 += m; u2 += n; }
                    u1 >>= 1;
                    u2 >>= 1;
                    u3 >>= 1;
                }

                if (t3.is_even() || u3 < t3) {
                    // Swap the u's with the t's
                    using std::swap;
                    swap(u1, t1);
                    swap(u2, t2);
                    swap(u3, t3);
                }
            } while (u3.is_even());

            while (u1 < t1 || u2 < t2) { u1 += m; u2 += n; }
            u1 -= t1; u2 -= t2; u3 -= t3;
        } while (t3 > zero);

        while (u1 >= m && u2 >= n) { u1 -= m; u2 -= n; }
    }

    BOOST_XINT_RAWINT u1, u2, u3;
};

BOOST_XINT_RAWINT_TPL
void gcd(BOOST_XINT_RAWINT &target, const BOOST_XINT_RAWINT &num1, const
    BOOST_XINT_RAWINT &num2)
{
    if (num1.is_zero() && num2.is_zero()) {
        target.set(0);
    } else if (num1.is_zero()) {
        target = num2;
    } else if (num2.is_zero()) {
        target = num1;
    } else {
        BOOST_XINT_RAWINT n(num1, false), m(num2, false);

        std::size_t k = 0;
        if (!getbit(n, k) && !getbit(m, k)) ++k;
        if (k != 0) { n >>= k; m >>= k; }

        gcd_core<Bits, Secure, Alloc> core(n, m);
        if (core.u3.is_zero()) {
            target.set(1);
            target <<= k;
        } else {
            target = core.u3;
            target <<= k;
        }
    }
}

BOOST_XINT_RAWINT_TPL
void lcm(BOOST_XINT_RAWINT &target, const BOOST_XINT_RAWINT &num1, const
    BOOST_XINT_RAWINT &num2)
{
    if (num1.is_zero() || num2.is_zero()) {
        target.set(0);
    } else {
        BOOST_XINT_RAWINT common;
        gcd(common, num1, num2);

        target = num1 * num2 / common;
        target.negative = false;
        target.trim();
    }
}

//! Calculates the modular inverse of n mod m, or (n^(-1)) mod m
//! Defined as b, where n*b corresponds to 1 (mod m)
BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT invmod(const BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT &m)
{
    if (m.is_zero() || m.negative) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_modulus());

    if (n.is_zero()) {
        return 0;
    } else if (n.negative) {
        BOOST_XINT_RAWINT target = invmod(n.abs(), m);
        if (!target.is_zero()) {
            target.negative = true;
            target += m;
        }
        target.trim();
        return target;
    } else {
        if (n.is_even() && m.is_even()) {
            // GCD != 1, no inverse possible.
            return 0;
        } else {
            gcd_core<Bits, Secure, Alloc> core(n, m);
            if (core.u3 != BOOST_XINT_RAWINT(1)) {
                // GCD != 1, no inverse possible.
                return 0;
            } else return core.u1;
        }
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_GCD_HPP
