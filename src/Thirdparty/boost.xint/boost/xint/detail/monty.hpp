
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
    \brief Contains the Montgomery Reduction functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_MONTY_HPP
#define BOOST_INCLUDED_XINT_MONTY_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

//! Returns the low digit of the inverse of a number.
BOOST_XINT_RAWINT_TPL
digit_t inverse0(const BOOST_XINT_RAWINT &n) {
    // Using the Dussé and Kalisk simplification
    doubledigit_t x = 2, y = 1;
    digit_t n0 = n[0];
    for (std::size_t i = 2; i <= bits_per_digit; ++i, x <<= 1)
        if (x < ((n0 * y) & ((x << 1) - 1))) y += x;
    return digit_t(x - y);
}

/*! \brief Returns the most efficient R value for a number and the library's
           internal representation. Used internally.
*/
BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT montgomeryR(const BOOST_XINT_RAWINT &n) {
    BOOST_XINT_RAWINT r;
    pow2(r, bits_per_digit * n.length);
    return r;
}

//! Returns the Montgomery form of a number. Used for testing.
BOOST_XINT_RAWINT_TPL
void toMontgomeryForm(BOOST_XINT_RAWINT &target, const BOOST_XINT_RAWINT &n,
    const BOOST_XINT_RAWINT &m)
{
    target = n * montgomeryR(m) % m;
}

//! Returns the integer from the Montgomery form of a number. Used for testing.
BOOST_XINT_RAWINT_TPL
void fromMontgomeryForm(BOOST_XINT_RAWINT &target, const BOOST_XINT_RAWINT &n,
    const BOOST_XINT_RAWINT &m)
{
    BOOST_XINT_RAWINT mont(montgomeryR(m)), inv = invmod(mont, m);
    target = n * inv % m;
}

/*! \brief The Montgomery equivalent of <code>mulmod(a, b, n)</code>. Used
           internally.
*/
BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT montgomeryMultiplyMod(const BOOST_XINT_RAWINT &a, const
    BOOST_XINT_RAWINT &b, const BOOST_XINT_RAWINT &n, digit_t nPrime0)
{
    // Using the Dussé and Kalisk simplification
    // Unstated parameter B is a power of two representing the number of values
    //   that a single digit can hold, i.e. digit_overflowbit
    // Unstated parameter L is the number of digits in the modulus, i.e.
    //   n.length
    // Unstated parameter r is B^L
    // nPrime0 is nPrime mod B, or digit zero of nPrime

    const std::size_t L(n.length);

    std::size_t i = 0;
    BOOST_XINT_RAWINT t(a * b);

    do {
        BOOST_XINT_RAWINT mi(digit_t(t[i] * nPrime0));
        mi *= n;
        mi <<= (bits_per_digit * i);
        t += mi;
    } while (++i < L);

    t >>= (bits_per_digit * L); // Fast divide by r

    if (t >= n) t -= n;
    return t;
}

// cMaxK sets the balance between memory/precalculations required and the number
// of calculations required for an exponentiation. Increasing it can only reduce
// the calculations by a small amount, whereas it increases the memory
// requirements and number of precalculations by an exponential amount. 8
// provides a good balance.
const std::size_t cMaxK = 8;
typedef boost::uint_t<cMaxK>::fast kbitdigit_t; // k bits have to fit into it
typedef std::vector<kbitdigit_t> vkbitdigit_t;
#define ddPowerOfTwo(p) (doubledigit_t(1) << p)

// The splitIntoKBitDigits function assumes that cMaxK <= bits_per_digit+1,
// it won't work properly if it isn't.
BOOST_STATIC_ASSERT(cMaxK <= bits_per_digit + 1);

// Make it a template so that the static variable won't cause problems in
// header-only mode.
template <typename T = void>
class TUTable {
    public:
    typedef std::pair<int, int> value_t;

    const value_t& operator[](std::size_t x) const { return mTable[x]; }

    static const TUTable& get() {
        // Construct a singleton instance on demand
        if (mPtr.get()==0) mPtr.reset(new TUTable);
        return *mPtr;
    }

    private:
    TUTable(): mTable(new value_t[ddPowerOfTwo(cMaxK)]) {
        value_t *p=&mTable[0], *pe=p+ddPowerOfTwo(cMaxK);
        *p++=std::make_pair(0, 0);
        int i=1;
        while (p!=pe) *p++=calculateValues(i++);
    }

    std::pair<int, int> calculateValues(int x) {
        int r=0;
        while (1) {
            if (x & 0x01) return std::make_pair(r, x);
            ++r;
            x >>= 1;
        }
    }

    static std::auto_ptr<TUTable<T> > mPtr;

    boost::scoped_array<value_t> mTable;
};

template <typename T>
std::auto_ptr<TUTable<T> > TUTable<T>::mPtr;

BOOST_XINT_RAWINT_TPL
int mostEfficientK(const BOOST_XINT_RAWINT &e) {
    doubledigit_t k = cMaxK, kTarget = log2(e) - 1;
    while (k > 1 && ((k - 1) * (k << ((k - 1) << 1))
        / (ddPowerOfTwo(k) - k - 1)) >= kTarget)
            --k;
    return int(k);
}

BOOST_XINT_RAWINT_TPL
std::vector<BOOST_XINT_RAWINT> precalculateOddPowersOfAa(const BOOST_XINT_RAWINT
    &a, const BOOST_XINT_RAWINT &r, const BOOST_XINT_RAWINT &n, std::size_t k)
{
    BOOST_XINT_RAWINT zero, one(1);
    BOOST_XINT_RAWINT aa = a * r % n, aSquared = a * a % n;

    std::vector<BOOST_XINT_RAWINT> rval;
    rval.reserve(std::size_t(ddPowerOfTwo(k)));
    rval.push_back(one);                // Anything to the zeroth power is one
    rval.push_back(aa);                 // Anything to the first power is itself

    for (doubledigit_t i = 3, ie = ddPowerOfTwo(k); i < ie; i += 2) {
        aa *= aSquared;
        aa %= n;
        rval.push_back(zero);           // Even powers not needed or calculated
        rval.push_back(aa);             // Odd power
    }

    return rval;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT montgomeryPowerMod(const BOOST_XINT_RAWINT &a, const
    BOOST_XINT_RAWINT &e, const BOOST_XINT_RAWINT &n)
{
    // 0 <= a < n, n is odd
    // Returns a^e mod n

    const TUTable<> &tuTable(TUTable<>::get());

    if (e.is_zero()) return 1;
    if (n.is_even()) exception_handler<>::call(__FILE__, __LINE__, exceptions::
        invalid_modulus("montgomeryPowerMod requires an odd modulus"));

    // Precalculate some values
    const std::size_t k(mostEfficientK(e));
    const BOOST_XINT_RAWINT r(montgomeryR(n));
    const digit_t nPrime0(inverse0(n));
    const std::vector<BOOST_XINT_RAWINT> oddPowersOfAa(
        precalculateOddPowersOfAa(a, r, n, k));

    // Slice the exponent (e) up into k-bit digits
    vkbitdigit_t eDigits;
    {
        bitqueue_t q;
        for (const digit_t *dig = e.digits(), *i = dig, *ie = i + e.length;
            i <ie; ++i) q.push(*i, bits_per_digit);

        // The bitqueue_t returns them low-bits-first, we need to start with the
        // high bits.
        while (!q.empty()) eDigits.push_back(kbitdigit_t(q.pop(k)));
    }

    while (eDigits.back() == 0) eDigits.pop_back();
    kbitdigit_t i = eDigits.back();
    eDigits.pop_back();

    BOOST_XINT_RAWINT pp;
    if (i == 0) {
        pp = r % n;
    } else {
        std::pair<int, int> tu = tuTable[i];
        pp = oddPowersOfAa[tu.second];
        while (tu.first-- > 0) pp = montgomeryMultiplyMod(pp, pp, n, nPrime0);
    }

    while (!eDigits.empty()) {
        i = eDigits.back();
        eDigits.pop_back();

        if (i == 0) {
            int t = int(k);
            while (t-- > 0) pp = montgomeryMultiplyMod(pp, pp, n, nPrime0);
        } else {
            std::pair<int, int> tu = tuTable[i];

            std::size_t s = k - tu.first;
            while (s-- > 0) pp = montgomeryMultiplyMod(pp, pp, n, nPrime0);

            pp = montgomeryMultiplyMod(pp, oddPowersOfAa[tu.second], n,
                nPrime0);

            while (tu.first-- > 0) pp = montgomeryMultiplyMod(pp, pp, n,
                nPrime0);
        }
    }
    return montgomeryMultiplyMod(pp, BOOST_XINT_RAWINT(1), n, nPrime0);
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_MONTY_HPP
