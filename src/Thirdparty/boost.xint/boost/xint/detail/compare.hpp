
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
    \brief Contains the definitions for the comparison functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_COMPARE_HPP
#define BOOST_INCLUDED_XINT_COMPARE_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

//! Compares equal-length sets of digits.
inline int compare(const digit_t *b1, const digit_t *b2, std::size_t len, bool
    equal_only)
{
    const detail::digit_t *b1i = b1 + len, *b2i = b2 + len;
    if (equal_only) {
        // If we only care whether it's equal or not, we'll start from the
        // lowest digit instead of the highest, on the assumption that it's more
        // likely to be different.
        while (b1 < b1i) if (*b1++ != *b2++) return 1;
    } else {
        while (b1i > b1)
            if (*(--b1i) != *(--b2i))
                return ((*b1i < *b2i) ? -1 : 1);
    }
    return 0; // They're identical.
}

//! The full-fat comparison function.
BOOST_XINT_RAWINT_TPL
int compare(const BOOST_XINT_RAWINT &b1, const BOOST_XINT_RAWINT &b2, bool
    ignoresign = false)
{
    bool invert_answer=false;
    if (!ignoresign) {
        if (b1.is_zero()) {
            if (b2.is_zero()) return 0;
            return (b2.negative ? 1 : -1);
        } else if (b2.is_zero()) {
            return (b1.negative ? -1 : 1);
        } else {
            if (b1.negative != b2.negative) return (b1.negative ? -1 : 1);
            if (b1.negative) invert_answer=true;
        }
    }

    int answer = 0;
    if (b1.length != b2.length) {
        answer = ((b1.length < b2.length) ? -1 : 1);
    } else {
        answer = compare(b1.digits(), b2.digits(), b1.length, false);
    }
    return (invert_answer ? -answer : answer);
}

//! Just compares for equality, returns non-zero if different.
BOOST_XINT_RAWINT_TPL
int compare_eq(const BOOST_XINT_RAWINT &b1, const BOOST_XINT_RAWINT &b2) {
    if (b1.negative != b2.negative) {
        // Have to test for is_zero here, because in some rare cases, negative
        // zero is allowed, and negative zero is equal to zero.
        return (!b1.is_zero() || !b2.is_zero() ? 1 : 0);
    }
    if (b1.length != b2.length) return 1;
    return compare(b1.digits(), b2.digits(), b1.length, true);
}

BOOST_XINT_RAWINT_TPL
bool operator==(const BOOST_XINT_RAWINT &num1, const BOOST_XINT_RAWINT &num2) {
    return compare_eq(num1, num2)==0; }
BOOST_XINT_RAWINT_TPL
bool operator!=(const BOOST_XINT_RAWINT &num1, const BOOST_XINT_RAWINT &num2) {
    return compare_eq(num1, num2)!=0; }
BOOST_XINT_RAWINT_TPL
bool operator<(const BOOST_XINT_RAWINT &num1, const BOOST_XINT_RAWINT &num2) {
    return compare(num1, num2)<0; }
BOOST_XINT_RAWINT_TPL
bool operator>(const BOOST_XINT_RAWINT &num1, const BOOST_XINT_RAWINT &num2) {
    return compare(num1, num2)>0; }
BOOST_XINT_RAWINT_TPL
bool operator<=(const BOOST_XINT_RAWINT &num1, const BOOST_XINT_RAWINT &num2) {
    return compare(num1, num2)<=0; }
BOOST_XINT_RAWINT_TPL
bool operator>=(const BOOST_XINT_RAWINT &num1, const BOOST_XINT_RAWINT &num2) {
    return compare(num1, num2)>=0; }

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_COMPARE_HPP
