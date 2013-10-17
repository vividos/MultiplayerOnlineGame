
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
    \brief Contains the definitions of the divide function, and its supporting
           functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_DIVIDE_HPP
#define BOOST_INCLUDED_XINT_DIVIDE_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
void divide_by_single_digit(BOOST_XINT_RAWINT &qtarget, BOOST_XINT_RAWINT
    &rtarget, const BOOST_XINT_RAWINT &d1, digit_t d2)
{
    std::size_t d1len = d1.length;
    const digit_t *d1digits = d1.digits();

    doubledigit_t a = 0;

    int m = int(d1len) - 1;
    const digit_t *d1p = d1digits + m;

    digit_t *qdig = qtarget.digits(m + 1, realloc::zero), *qp = qdig + m, *qe =
        qdig + qtarget.max_length();
    int i = m;
    for (; qp >= qe && i >= 0; --i, --d1p, --qp) {
        a |= *d1p;
        a = ((a % d2) << bits_per_digit);
    }
    for (; i >= 0; --i, --d1p, --qp) {
        a |= *d1p;
        *qp = static_cast<digit_t>(a / d2);
        a = ((a % d2) << bits_per_digit);
    }
    qtarget.length = (std::min<size_t>)(m + 1, qtarget.max_length());
    qtarget.trim();

    rtarget.digits(1, realloc::ignore)[0] = static_cast<digit_t>(a >>
        bits_per_digit);
    rtarget.length = 1;
    rtarget.trim();
}

BOOST_XINT_RAWINT_TPL
void sub_divide2(BOOST_XINT_RAWINT &qtarget, BOOST_XINT_RAWINT &rtarget, const
    BOOST_XINT_RAWINT &d1, const BOOST_XINT_RAWINT &d2)
{
    typedef raw_integer_t<0, Secure, Alloc> BOOST_XINT_UNLIMITED;
    assert(d2[d2.length - 1] >= digit_overflowbit / 2);

    const digit_t *byDigits = d2.digits();

    std::size_t n = d2.length, m = d1.length - n;
    std::size_t i = m + n, j = m;

    digit_t *qdigits = qtarget.digits(j + 1, realloc::ignore);
    qtarget.length = (std::min)(j + 1, qtarget.max_length());
    rtarget = d1;
    BOOST_XINT_UNLIMITED r2(0, 3), qq;
    do {
        doubledigit_t ri = (doubledigit_t(rtarget.get_digit(i)) <<
            bits_per_digit) + rtarget.get_digit(i - 1);
        doubledigit_t q = (std::min<doubledigit_t>)(ri / byDigits[n - 1],
            digit_mask);

        while (1) {
            // We need three digits here, a doubledigit_t alone won't suffice.
            {
                doubledigit_t tmp = ri - (q * byDigits[n - 1]);
                digit_t *r2dig = r2.digits(3, realloc::ignore);
                *r2dig++ = rtarget[i - 2];
                *r2dig++ = static_cast<digit_t>(tmp & digit_mask);
                tmp >>= bits_per_digit;
                *r2dig++ = static_cast<digit_t>(tmp);
                r2.trim();
            }

            qq.set_unsigned(q);
            if (BOOST_XINT_UNLIMITED(byDigits[n - 2]) * qq <= r2) break;
            --q;
        }

        BOOST_XINT_RAWINT bq(d2 * BOOST_XINT_RAWINT(q));
        if (rtarget < bq) {
            --q;
            bq -= d2;
        }

        bq <<= bits_per_digit * (i - n);
        rtarget -= bq;

        qdigits[j--] = digit_t(q);
    } while (--i >= n);
    rtarget.trim();
    qtarget.trim();
}

//! This function weeds out single-digit division (for more efficient
//! processing), normalizes the values if necessary, then calls sub_divide2 for
//! the actual work.
BOOST_XINT_RAWINT_TPL
void sub_divide(BOOST_XINT_RAWINT &qtarget, BOOST_XINT_RAWINT &rtarget, const
    BOOST_XINT_RAWINT &d1, const BOOST_XINT_RAWINT &d2)
{
    if (d2.length == 1) {
        divide_by_single_digit(qtarget, rtarget, d1, d2[0]);
    } else {
        // The normalization step
        digit_t normalizer = static_cast<digit_t>(digit_overflowbit /
            (doubledigit_t(d2[d2.length - 1]) + 1));
        if (normalizer > 1) {
            const BOOST_XINT_RAWINT norm(normalizer);
            sub_divide2(qtarget, rtarget, d1 * norm, d2 * norm);

            // Denormalization step. This requires division by a single digit_t.
            BOOST_XINT_RAWINT tmp1, tmp2(rtarget);
            divide_by_single_digit(rtarget, tmp1, tmp2, normalizer);
        } else sub_divide2(qtarget, rtarget, d1, d2);
    }
}

//! This function handles signs and trivial cases, and calls sub_divide for the
//! heavy lifting.
BOOST_XINT_RAWINT_TPL
void divide(BOOST_XINT_RAWINT &qtarget, BOOST_XINT_RAWINT &rtarget, const
    BOOST_XINT_RAWINT &d1, const BOOST_XINT_RAWINT &d2)
{
    int sign1 = (d1.is_zero() ? 0 : d1.negative ? -1 : 1);
    int sign2 = (d2.is_zero() ? 0 : d2.negative ? -1 : 1);
    if (sign2 == 0) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::divide_by_zero());

    int comp = compare(d1, d2, true);
    if (comp < 0) {
        qtarget.set(0);
        rtarget = d1;
    } else if (comp == 0) {
        qtarget.set(sign1 != sign2 ? -1 : 1);
        rtarget.set(0);
    } else if (sign1 < 0 && sign2 < 0) {
        sub_divide(qtarget, rtarget, d1.negate(), d2.negate());
        qtarget.negative = false;
        rtarget.negative = true;
    } else if (sign1 < 0) {
        sub_divide(qtarget, rtarget, d1.negate(), d2);
        qtarget.negative = rtarget.negative = true;
    } else if (sign2 < 0) {
        sub_divide(qtarget, rtarget, d1, d2.negate());
        qtarget.negative = true;
        rtarget.negative = false;
    } else {
        sub_divide(qtarget, rtarget, d1, d2);
        qtarget.negative = rtarget.negative = false;
    }
    rtarget.trim();
    qtarget.trim();
}

BOOST_XINT_RAWINT_TPL
typename BOOST_XINT_RAWINT::divide_t divide(const BOOST_XINT_RAWINT &d1, const
    BOOST_XINT_RAWINT &d2)
{
    BOOST_XINT_RAWINT q, r;
    divide(q, r, d1, d2);
    return typename BOOST_XINT_RAWINT::divide_t(q, r);
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator/=(const BOOST_XINT_RAWINT &b) {
    BOOST_XINT_RAWINT remainder, tmp(*this);
    divide(*this, remainder, tmp, b);
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator%=(const BOOST_XINT_RAWINT &b) {
    BOOST_XINT_RAWINT quotient, tmp(*this);
    divide(quotient, *this, tmp, b);
    if (negative) *this += b.abs();
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator/(const BOOST_XINT_RAWINT &n1, const BOOST_XINT_RAWINT
    &n2)
{
    BOOST_XINT_RAWINT quotient, remainder;
    divide(quotient, remainder, n1, n2);
    return quotient;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator%(const BOOST_XINT_RAWINT &n1, const BOOST_XINT_RAWINT
    &n2)
{
    BOOST_XINT_RAWINT quotient, remainder;
    divide(quotient, remainder, n1, n2);
    if (remainder.negative) remainder += n2.abs();
    return remainder;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_DIVIDE_HPP
