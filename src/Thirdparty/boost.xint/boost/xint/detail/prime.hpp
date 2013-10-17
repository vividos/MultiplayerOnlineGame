
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
    \brief Contains the function definitions for the prime number functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_PRIME_HPP
#define BOOST_INCLUDED_XINT_PRIME_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

inline std::vector<int> sieveOfEratosthenes(int upTo) {
    std::vector<int> sieve;
    sieve.reserve(upTo);

    // Zero and one aren't prime, by definition.
    sieve.push_back(0);
    sieve.push_back(0);

    for (int p = 2; p < upTo; ++p) sieve.push_back(p);

    std::vector<int> ret;

    int *p = &sieve[0], *e = p + upTo;
    while (p < e) {
        if (*p == 0) { ++p; continue; }
        ret.push_back(*p);

        int *p2 = p + (*p);
        while (p2 < e) { *p2 = 0; p2 += *p; }

        ++p;
    }

    return ret;
}

// The Miller-Rabin Probabilistic Test of Primality
BOOST_XINT_RAWINT_TPL
int isProbablePrimeBaseB(const BOOST_XINT_RAWINT &n, const BOOST_XINT_RAWINT &b,
    callback_t callback)
{
    const BOOST_XINT_RAWINT one(1), nMinus1(n - one);

    // Find r and a satisfying: n - 1 = 2^a * r, r odd
    BOOST_XINT_RAWINT r(nMinus1);
    unsigned long a = 0;
    while (r.is_even()) { r >>= 1; ++a; }

    // We check the congruence class of b^((2^i)r) % n for each i
    // from 0 to a - 1. If any one is correct, then n passes.
    // Otherwise, n fails.
    BOOST_XINT_RAWINT test = powmod(b, r, n);
    if (test == one || test == nMinus1)
        return 1; // Better than 3/4 chance it's prime

    while (a-- > 0) {
        test = sqrmod(test, n);
        if (test == nMinus1) return 1;
    }

    if (callback && !callback()) return -1;
    return 0;
}

template <bitsize_t Bits, bool Secure, class Alloc, class GenType>
int is_probably_prime(const BOOST_XINT_RAWINT &n, GenType &gen, callback_t
    callback)
{
    BOOST_XINT_RAWINT tmp(2);
    if (n < tmp) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_argument("xint::is_probably_prime cannot test "
            "numbers below positive 2"));

    // First we trial-divide it by the primes below 2000
    static const std::vector<int> low_primes(sieveOfEratosthenes(2000));
    const BOOST_XINT_RAWINT zero;
    std::vector<int>::const_iterator i = low_primes.begin(), ie =
        low_primes.end();
    for (; i != ie; ++i) {
        tmp.set_unsigned(*i);
        if ((n % tmp) == zero)
            return (n == tmp);
    }

    // Run the number through the Miller-Rabin Probabilistic Test of Primality
    // a few times to see if it's actually (probably) prime.
    for (int count = 0; count < 5; ++count) {
        unsigned int k = gen();
        tmp.set_unsigned(k);
        int isP = isProbablePrimeBaseB(n, tmp, callback);
        if (isP <= 0) return isP;
    }
    return 1; // Appears to be prime!
}

BOOST_XINT_RAWINT_TPL
template <typename GenType>
BOOST_XINT_RAWINT BOOST_XINT_RAWINT::random_prime(GenType &gen, std::size_t
    size_in_bits, callback_t callback)
{
    if (size_in_bits < 2) exception_handler<>::call(__FILE__, __LINE__,
        exceptions::invalid_argument("cannot create prime numbers smaller than "
        "two bits"));

    // Call the callback for the first time
    if (callback && !callback()) return 0;

    // If, by rare chance, the number we're testing gets larger than the number
    // of bits requested, we have to start over with a new random number of the
    // proper size.
    BOOST_XINT_RAWINT pe;
    pow2(pe, size_in_bits + 1);

    const BOOST_XINT_RAWINT two(2);
    while (1) {
        BOOST_XINT_RAWINT target = random_by_size(gen, size_in_bits, true, true,
            false);
        while (target < pe) {
            int r = is_probably_prime(target, gen, callback);
            if (r < 0) return 0;
            if (r == 1) return target;
            target += two;
        }
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_PRIME_HPP
