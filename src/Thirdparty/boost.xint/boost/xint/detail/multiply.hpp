
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
    \brief Contains the multiplication and squaring functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_MULTIPLY_HPP
#define BOOST_INCLUDED_XINT_MULTIPLY_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT square(const BOOST_XINT_RAWINT &n) {
    const digit_t *ndigits = n.digits();
    std::size_t nlen = n.length;

    BOOST_XINT_RAWINT target;
    digit_t *rdigits = target.digits(n.length * 2 + 1, realloc::zero), *rmax =
        rdigits + target.length;

    // Calculate the product of digits of unequal index
    std::size_t i = 0;
    doubledigit_t c;
    do {
        const doubledigit_t ii = ndigits[i];
        const digit_t *jp = ndigits + i + 1;
        digit_t *rp = rdigits + i + (i + 1), *rpe = rdigits + i + nlen;
        if (rpe > rmax) rpe = rmax;

        c = 0;
        while (rp < rpe) {
            doubledigit_t t = ii * *jp++ + *rp + c;
            *rp++ = static_cast<digit_t>(t);
            c = (t >> bits_per_digit);
        }
        if (rp < rmax) *rp = static_cast<digit_t>(c);
    } while (++i < nlen - 1);

    // Multiplication of inner products by two
    c = 0;
    digit_t *rp = rdigits + 1, *rpe = rdigits + (2 * nlen) - 1;
    if (rpe > rmax) rpe = rmax;

    if (rp < rmax) {
        do {
            doubledigit_t t = (doubledigit_t(*rp) << 1) + c;
            *rp++ = static_cast<digit_t>(t);
            c = (t >> bits_per_digit);
        } while (rp < rpe);
        if (rp < rmax) *rp = static_cast<digit_t>(c);
    }

    // Addition of inner squares
    const digit_t *ip = ndigits, *ipe = ndigits + nlen;
    rp = rdigits;
    c = 0;
    do {
        doubledigit_t t = doubledigit_t(*ip) * *ip + *rp + c;
        *rp++ = static_cast<digit_t>(t);
        if (rp >= rmax) break;
        c = (t >> bits_per_digit);

        t = c + *rp;
        *rp++ = static_cast<digit_t>(t);
        if (rp >= rmax) break;
        c = (t >> bits_per_digit);
    } while (++ip < ipe);
    if (rp < rmax) *rp += static_cast<digit_t>(c);

    // No need to change length
    target.negative = false;
    target.trim();
    return target;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT multiply(const BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT
    &by)
{
    if (n.is_zero() || by.is_zero()) return 0;

    if (n.same_storage(by)) {
        if (n.negative != by.negative) {
            return BOOST_XINT_RAWINT(square(n), true);
        } else {
            return square(n);
        }
    }

    BOOST_XINT_RAWINT target;
    digit_t *adigits = target.digits(n.length + by.length, realloc::zero),
        *ae = adigits + target.max_length();
    std::size_t maxdigit = target.max_length(), nlen = n.length, bylen =
        by.length;

    // Now multiply the digits, starting at the least-significant digit.
    const digit_t *d1 = n.digits(), *d1e = d1 + (std::min)(nlen, maxdigit);
    for (int digit1 = 0; d1 < d1e; ++digit1, ++d1) {
        const digit_t *d2 = by.digits(), *d2e = d2 + (std::min)(bylen, maxdigit
            - digit1);
        for (int digit2 = 0; d2 < d2e; ++digit2, ++d2) {
            // First multiply the two digits
            doubledigit_t t = doubledigit_t(*d1) * *d2;

            // Now add the result to the answer.
            int carry = 0;
            digit_t *a = adigits + digit1 + digit2;
            doubledigit_t addt = doubledigit_t(*a) + (t & digit_mask);
            if (addt >= digit_overflowbit) carry = 1;
            *a++=static_cast<digit_t>(addt);

            if (a < ae) {
                addt = *a + ((t >> bits_per_digit) & digit_mask) + carry;
                if (addt >= digit_overflowbit) carry = 1; else carry = 0;
                *a++ = static_cast<digit_t>(addt);

                while (carry && a < ae) {
                    addt = *a + 1;
                    *a++ = static_cast<digit_t>(addt);
                    if (addt < digit_overflowbit) break;
                }
            }
        }
    }

    // No need to change length
    target.negative = (n.negative != by.negative);
    target.trim();
    return target;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator*=(const BOOST_XINT_RAWINT &by) {
    *this = multiply(*this, by);
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator*(const BOOST_XINT_RAWINT &n1, const BOOST_XINT_RAWINT
    &n2)
{
    return multiply(n1, n2);
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_MULTIPLY_HPP
