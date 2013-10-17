
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
    \brief Contains the definitions of the addition and subtraction functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_ADDSUBTRACT_HPP
#define BOOST_INCLUDED_XINT_ADDSUBTRACT_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
void sub_increment(BOOST_XINT_RAWINT &n, bool absolute_value = false);
BOOST_XINT_RAWINT_TPL
void sub_decrement(BOOST_XINT_RAWINT &n, bool absolute_value = false);

BOOST_XINT_RAWINT_TPL
void sub_add(BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT &n2) {
    digit_t *ndig = n.digits((std::max)(n.length, n2.length) + 1,
        realloc::extend), *t = ndig, *te = t + n.length;
    const digit_t *p = n2.digits(), *pe = p + (std::min)(n2.length, n.length);

    digit_t carry = 0;
    while (p != pe) {
        doubledigit_t tmp = doubledigit_t(*t) + *p++ + carry;
        if (tmp >= digit_overflowbit) {
            carry = 1;
            *t++ = static_cast<digit_t>(tmp - digit_overflowbit);
        } else {
            carry = 0;
            *t++ = static_cast<digit_t>(tmp);
        }
    }

    if (carry) {
        while (t != te) {
            if (*t == digit_mask) *t++ = 0;
            else { *t++ += 1; break; }
        }
    }

    n.length = (std::max)(n.length, std::size_t(t - ndig));
    n.trim();
}

BOOST_XINT_RAWINT_TPL
void sub_subtract(BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT &n2) {
    bool swap = (compare(n, n2, true) < 0);

    digit_t *ndig = n.digits((std::max)(n.length, n2.length), realloc::extend),
        *t = ndig, *te = t + n.length;
    const digit_t *p = n2.digits(), *pe = p + (std::min)(n2.length, n.length);

    // Now subtract the digits, starting at the least-significant one.
    digit_t borrow = 0;
    if (swap) {
        // abs(n2) is greater than abs(n). Requires slightly different handling.
        while (p != pe) {
            doubledigit_t tmp = digit_overflowbit + *p++ - *t - borrow;
            if (tmp < digit_overflowbit) {
                borrow = 1;
                *t++ = static_cast<digit_t>(tmp);
            } else {
                borrow = 0;
                *t++ = static_cast<digit_t>(tmp - digit_overflowbit);
            }
        }

        n.length = std::size_t(t - ndig);
        n.negative = !n.negative;
    } else {
        while (p != pe) {
            doubledigit_t tmp = digit_overflowbit + *t - *p++ - borrow;
            if (tmp < digit_overflowbit) {
                borrow = 1;
                *t++ = static_cast<digit_t>(tmp);
            } else {
                borrow = 0;
                *t++ = static_cast<digit_t>(tmp - digit_overflowbit);
            }
        }

        if (borrow) {
            while (t != te) {
                if (*t == 0) *t++ = digit_mask;
                else { *t++ -= 1; borrow = 0; break; }
            }
        }
        n.length = (std::max)(n.length, std::size_t(t - ndig));
    }

    assert(borrow == 0);
    n.trim();
}

BOOST_XINT_RAWINT_TPL
void sub_increment(BOOST_XINT_RAWINT &n, bool absolute_value) {
    if (n.is_zero()) {
        n.set(1);
    } else if (!absolute_value && n.negative) {
        sub_decrement(n, true);
    } else {
        std::size_t overflow = (n.digits()[n.length - 1] == digit_mask ? 1 : 0);
        digit_t *d = n.digits(n.length + overflow, realloc::extend), *p = d, *pe
            = p + n.length;
        while (p < pe) {
            if (*p == digit_mask) *p++ = 0;
            else { *p++ += 1; break; }
        }
        n.trim();
    }
}

BOOST_XINT_RAWINT_TPL
void sub_decrement(BOOST_XINT_RAWINT &n, bool absolute_value) {
    if (n.is_zero()) {
        n.set(-1);
    } else if (!absolute_value && n.negative) {
        sub_increment(n, true);
    } else {
        digit_t *p = n.digits(0), *pe = p + n.length;

        while (p < pe) {
            if (*p == 0) *p++ = digit_mask;
            else { *p++ -= 1; break; }
        }
        n.trim();
    }
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT &BOOST_XINT_RAWINT::operator++() {
    sub_increment(*this);
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT &BOOST_XINT_RAWINT::operator--() {
    sub_decrement(*this);
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT BOOST_XINT_RAWINT::operator++(int) {
    BOOST_XINT_RAWINT r(*this);
    sub_increment(*this);
    return r;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT BOOST_XINT_RAWINT::operator--(int) {
    BOOST_XINT_RAWINT r(*this);
    sub_decrement(*this);
    return r;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT &BOOST_XINT_RAWINT::operator+=(const BOOST_XINT_RAWINT &n) {
    if (!n.is_zero()) {
        if (is_zero()) {
            *this = n;
        } else if (negative != n.negative) {
            sub_subtract(*this, n.negate());
        } else {
            sub_add(*this, n);
        }
    }
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT &BOOST_XINT_RAWINT::operator-=(const BOOST_XINT_RAWINT &n) {
    if (!n.is_zero()) {
        if (is_zero()) {
            *this = n.negate();
        } else if (negative != n.negative) {
            sub_add(*this, n.negate());
        } else {
            sub_subtract(*this, n);
        }
    }
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator+(BOOST_XINT_RAWINT n1, const BOOST_XINT_RAWINT &n2) {
    n1 += n2;
    return n1;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator-(BOOST_XINT_RAWINT n1, const BOOST_XINT_RAWINT &n2) {
    n1 -= n2;
    return n1;
}

// note: once the test system is up and running, rewrite add/subtract code to write to a separate raw_integer & return it, for greater efficiency.

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_ADDSUBTRACT_HPP
