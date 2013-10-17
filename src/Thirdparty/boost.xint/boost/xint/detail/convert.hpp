
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
    \brief Contains the conversion functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_CONVERT_HPP
#define BOOST_INCLUDED_XINT_CONVERT_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

inline const char *letters() {
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

inline char nToChar(int n, bool upperCase) {
    if (n < 10) return char(n + '0');
    else return letters()[n - 10 + (upperCase ? 26 : 0)];
}

template <typename CharT>
int nFromChar(CharT c) {
    // Digits are easy, they're guaranteed contiguous.
    if (c >= '0' && c <= '9') return c - '0';

    // Letters might not be.
    if (c <= (std::numeric_limits<char>::max)()) {
        const char *ll = letters(), *p = strchr(ll, char(c));
        if (p != 0) return ((p - ll) % 26) + 10;
    }

    return -1;
}

template <typename CharT>
bool is_hex_digit(CharT c) {
    int n = nFromChar(c);
    return (n >= 0 && n <= 0x0f);
}

template <typename charT, bitsize_t Bits, bool Secure, class Alloc>
std::basic_string<charT> to_string(const BOOST_XINT_RAWINT &n, std::size_t base
    = 10, bool uppercase = false)
{
    if (base < 2 || base > 36) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_base());

    std::basic_ostringstream<charT> out;
    if (n.is_zero()) {
        out << "0";
        return out.str();
    }

    if (base == 2 || base == 4 || base == 16) {
        // Fast no-division version, useful for debugging division and for cases
        // where maximum speed is necessary.
        const digit_t *firstDigit = n.digits(), *lastDigit = firstDigit +
            n.length - 1;

        if (n.negative) out << '-';

        int bits_per_base_b_digit = 0;
        while (base > 1) { base = base >> 1; ++bits_per_base_b_digit; }
        const digit_t mask = (doubledigit_t(1) << bits_per_base_b_digit) - 1;

        // Skip any leading zeros
        const digit_t *d = lastDigit;
        int digitShift = (bits_per_digit - bits_per_base_b_digit);
        while (digitShift >= 0 && ((*d >> digitShift) & mask) == 0)
            digitShift -= bits_per_base_b_digit;

        do {
            while (digitShift >= 0) {
                out << nToChar((*d >> digitShift) & mask, uppercase);
                digitShift -= bits_per_base_b_digit;
            }

            digitShift = (bits_per_digit - bits_per_base_b_digit);
        } while (d-- != firstDigit);

        return out.str();
    } else {
        // ATTN: for when there's nothing more pressing to do
        // This could be made a lot more efficient for very large numbers, by
        // dividing n by base raised to whatever number of digits will fit into
        // a doubledigit_t, then just doing single-digit divides for that many
        // digits before going back to n for another chunk. That would be
        // premature optimization though, so I'm just making this note instead.
        // If someone can show me a use-case where more efficiency is needed in
        // this function, I'll add it later.

        BOOST_XINT_RAWINT shift;
        shift.set_unsigned(base);
        typename BOOST_XINT_RAWINT::divide_t a(divide(n.abs(), shift));
        do {
            out << nToChar(a.remainder[0], uppercase);
            a = divide(a.quotient, shift);
        } while (!a.quotient.is_zero());
        if (!a.remainder.is_zero()) out << nToChar(a.remainder[0], uppercase);

        if (n.negative) out << '-';

        std::basic_string<charT> rval = out.str();
        std::reverse(rval.begin(), rval.end());
        return rval;
    }
}

BOOST_XINT_RAWINT_TPL
template <typename charT>
void BOOST_XINT_RAWINT::from_string(const charT *str, const charT*& endptr,
    std::size_t base)
{
    bool negate = false;
    const charT *c = str;
    while (isspace(*c)) ++c;

    if (*c == '+') ++c;
    else if (*c == '-') { negate = true; ++c; }

    if (base == autobase) {
        if (*c == '0') {
            ++c;
            if (*c == 'x' || *c == 'X') {
                ++c;
                base = 16;
            } else base = 8;
        } else base = 10;
    }

    if (base < 2 || base > 36) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_base());
    if (*c == 0) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_digit("No valid digits"));

    std::basic_string<charT> tstr;
    if (negate) tstr.push_back('-');
    if (base <= 10) {
        const charT p = charT('0' + base);
        while (*c >= '0' && *c < p)
            tstr.push_back(*c++);
    } else while (nFromChar(*c) >= 0) tstr.push_back(*c++);
    endptr = c;

    from_string(tstr, base);
}

BOOST_XINT_RAWINT_TPL
template <typename charT>
void BOOST_XINT_RAWINT::from_string(const charT *str, std::size_t base) {
    bool negate = false;
    const charT *c = str;
    if (*c == '+') ++c;
    else if (*c == '-') { negate = true; ++c; }

    if (base == autobase) {
        if (*c == '0') {
            ++c;
            if (*c == 'x' || *c == 'X') {
                ++c;
                base = 16;
            } else base = 8;
        } else base = 10;
    }

    if (base < 2 || base > 36) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_base());
    if (*c == 0) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_digit("No valid digits"));

    // ATTN: for when there's nothing more pressing to do
    // This function could use the same efficiency improvements that to_string
    // uses, but there's even less need for them here. Show me a use-case where
    // they're needed and I'll add them; until then, this will suffice.

    BOOST_XINT_RAWINT shift, digit;
    shift.set_unsigned(base);

    set(0);
    while (*c) {
        int val = nFromChar(*c);
        if (val < 0) exception_handler<>::call(__FILE__, __LINE__,
            exceptions::invalid_digit("encountered non-alphanumeric character "
            "in string"));
        digit.set(val);

        if (digit >= shift) exception_handler<>::call(__FILE__, __LINE__,
            exceptions::invalid_digit("encountered digit greater than base "
            "allows"));

        *this *= shift;
        *this += digit;
        ++c;
    }

    negative = negate;
    trim();
}

BOOST_XINT_RAWINT_TPL
template <typename charT, typename traitsT, typename allocT>
void BOOST_XINT_RAWINT::from_string(const std::basic_string<charT, traitsT,
    allocT>& str, std::size_t base)
{
    from_string(str.c_str(), base);
}

BOOST_XINT_RAWINT_TPL
xint::binary_t to_binary(const BOOST_XINT_RAWINT &n, std::size_t bits = 0) {
    if (bits > std::size_t(std::numeric_limits<unsigned char>::digits))
        exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_argument("can't fit that many bits into an "
        "unsigned character on this system"));
    if (bits == 0) bits = std::numeric_limits<unsigned char>::digits;

    bitqueue_t bitqueue;
    const digit_t *s = n.digits(), *se = s + n.length;
    while (s != se) bitqueue.push(*s++, bits_per_digit);

    xint::binary_t target;
    while (!bitqueue.empty()) target.push_back(static_cast<unsigned char>
      (bitqueue.pop(bits)));
    while (!target.empty() && target.back()==0) target.pop_back();
    return target;
}

BOOST_XINT_RAWINT_TPL
void BOOST_XINT_RAWINT::from_binary(const xint::binary_t &b, std::size_t bits) {
    if (bits > std::size_t(std::numeric_limits<unsigned char>::digits))
        exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_argument("can't fit that many bits into an "
        "unsigned character on this system"));
    if (bits == 0) bits = std::numeric_limits<unsigned char>::digits;

    bitqueue_t bitqueue;
    for (xint::binary_t::const_iterator s = b.begin(), se = b.end(); s != se;
        ++s) bitqueue.push(*s, bits);

    digit_t *d = digits(bits_to_digits(b.size() * bits), realloc::ignore), *t =
        d, *te = t + max_length();
    while (t != te && !bitqueue.empty())
      *t++ = static_cast<digit_t>(bitqueue.pop(bits_per_digit));
    length = (t - d);
    trim();
}

template <typename T, bitsize_t Bits, bool Secure, class Alloc>
T to(const BOOST_XINT_RAWINT &n, typename boost::enable_if<boost::is_integral<T>
    >::type* = 0)
{
    using std::numeric_limits;

    static const BOOST_XINT_RAWINT tmin((numeric_limits<T>::min)());
    static const BOOST_XINT_RAWINT tmax((numeric_limits<T>::max)());
    if (n < tmin || n > tmax) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::too_big("value out of range for requested conversion"));

    // Workaround for MSVC8's C4309 warning, "truncation of constant value"
    doubledigit_t shift_tmp = digit_overflowbit;
    T rval = 0, shift = T(shift_tmp);
    for (std::size_t x = 0; x < n.length; ++x) {
        if (sizeof(T) > sizeof(digit_t)) rval *= shift;
        rval += static_cast<T>(n[n.length - x - 1]);
    }
    if (n.negative) rval *= static_cast<T>(-1);
    return rval;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_CONVERT_HPP
