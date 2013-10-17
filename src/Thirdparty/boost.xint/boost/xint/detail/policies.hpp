
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
    \brief Contains the policy classes.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_POLICY_HPP
#define BOOST_INCLUDED_XINT_POLICY_HPP

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

////////////////////////////////////////////////////////////////////////////////
// Allocation

struct realloc {
    enum strategy { ignore, zero, copy, extend };
};

template <bitsize_t Bits, bool Secure, class Alloc>
class allocator_t {
    BOOST_STATIC_ASSERT(sizeof(typename Alloc::value_type) == sizeof(digit_t));

    public:
    static magnitude_t *alloc(std::size_t size, bool readonly = false);
    static magnitude_t *realloc_and_uniquify(magnitude_t *old, std::size_t
        newsize = 0, realloc::strategy strategy = realloc::copy);
    static magnitude_t *duplicate(magnitude_t *source, std::size_t newsize = 0);
    static void dealloc(magnitude_t *p);

    static void attach(magnitude_t *p);
    static void detach(magnitude_t *p);

    private:
    static Alloc allocator;
    static const std::size_t fixed_length;
    static const std::size_t magnitude_datasize;
};

template <bitsize_t Bits, bool Secure, class Alloc>
    Alloc allocator_t<Bits, Secure, Alloc>::allocator;
template <bitsize_t Bits, bool Secure, class Alloc>
    const std::size_t allocator_t<Bits, Secure, Alloc>::fixed_length = (Bits +
    bits_per_digit - 1) / bits_per_digit;
template <bitsize_t Bits, bool Secure, class Alloc>
    const std::size_t allocator_t<Bits, Secure, Alloc>::magnitude_datasize =
    (sizeof(magnitude_t) + sizeof(digit_t) - 1) / sizeof(digit_t);

////////////////////////////////////////////////////////////////////////////////
// Fixed- or variable-length

template <bitsize_t Bits, bitsize_t Modded = Bits % bits_per_digit>
class fixedlength_t {
    public:
    static void trim(magnitude_t *p) {
        // Only valid if Bits % bits_per_digit is non-zero.
        p->digits[last_digit] &= mask;
    }

    private:
    static const std::size_t last_digit;
    static const digit_t mask;
};

template <bitsize_t Bits, bitsize_t Modded>
const std::size_t fixedlength_t<Bits, Modded>::last_digit = ((Bits +
    bits_per_digit - 1) / bits_per_digit) - 1;
template <bitsize_t Bits, bitsize_t Modded>
const digit_t fixedlength_t<Bits, Modded>::mask = digit_t((doubledigit_t(1) <<
    (Bits % bits_per_digit)) - 1);

template <bitsize_t Bits>
class fixedlength_t<Bits, 0> {
    public:
    static void trim(magnitude_t*) { }
};

////////////////////////////////////////////////////////////////////////////////
// Function Definitions

template <bitsize_t Bits, bool Secure, class Alloc>
magnitude_t *allocator_t<Bits, Secure, Alloc>::alloc(std::size_t size, bool
    readonly)
{
    if (fixed_length) size = fixed_length;
    else if (size < minimum_digits) size = minimum_digits;
    const std::size_t count = size - minimum_digits + magnitude_datasize;

    digit_t *storage = 0;
    BOOST_XINT_TRY {
        storage = allocator.allocate(count);
        if (readonly) return new(storage) magnitude_t(size, readonly, 0);
        else return new(storage) magnitude_t(size, readonly);
    } BOOST_XINT_CATCH_BADALLOC {
        exception_handler<>::call(__FILE__, __LINE__,
            exceptions::overflow_error("Out of memory allocating integer"));
    } BOOST_XINT_CATCH_E {
        // To make it exception-safe, we've got to ensure that the allocated
        // data is properly disposed of if there's an exception, before passing
        // the exception on.
        if (storage) allocator.deallocate(storage, size);
        exception_handler<>::call(__FILE__, __LINE__, e);
    }

    // It will never get here, but the compilers can't tell that.
    return 0;
}

template <bitsize_t Bits, bool Secure, class Alloc>
magnitude_t *allocator_t<Bits, Secure, Alloc>::realloc_and_uniquify(magnitude_t
    *old, std::size_t newsize, realloc::strategy strategy)
{
    if (fixed_length) {
        newsize = fixed_length;
    } else if (newsize == 0) {
        if (old) newsize = old->max_length;
        else newsize = minimum_digits;
    } else if (newsize < minimum_digits) newsize = minimum_digits;

    magnitude_t *r = old;
    if (old == 0 || newsize > old->max_length || old->copies() > 1) {
        r = alloc(newsize);
        attach(r);

        if (strategy == realloc::ignore) {
            // Do nothing.
        } else if ((strategy == realloc::extend || strategy == realloc::copy) &&
            old != 0)
        {
            const digit_t *s = old->digits, *se = s + old->max_length;
            digit_t *t = r->digits, *te = t + r->max_length;
            while (s != se && t != te) *t++ = *s++;
        } else {
            // It's realloc::zero, old == 0. Zero it.
            digit_t *t = r->digits, *te = t + r->max_length;
            while (t != te) *t++ = 0;
        }

        detach(old);
    } else if (strategy == realloc::zero) {
        digit_t *t = r->digits, *te = t + r->max_length;
        while (t != te) *t++ = 0;
    }

    return r;
}

template <bitsize_t Bits, bool Secure, class Alloc>
magnitude_t *allocator_t<Bits, Secure, Alloc>::duplicate(magnitude_t *source,
    std::size_t newsize)
{
    attach(source);
    return realloc_and_uniquify(source, newsize, realloc::copy);
}

template <bitsize_t Bits, bool Secure, class Alloc>
void allocator_t<Bits, Secure, Alloc>::dealloc(magnitude_t *data) {
    if (data == 0) return;

    const std::size_t len = (data->max_length + magnitude_datasize -
        minimum_digits);

    // Since the data was initialized with placement-new, we have to manually
    // call the destructor.
    data-> ~magnitude_t();

    // Finally, dispose of it.
    digit_t *p = reinterpret_cast<digit_t*>(data);
    if (Secure) memset(p, 0, len * sizeof(digit_t));
    allocator.deallocate(p, len);
}

template <bitsize_t Bits, bool Secure, class Alloc>
void allocator_t<Bits, Secure, Alloc>::attach(magnitude_t *p) {
    p->inc();
}

template <bitsize_t Bits, bool Secure, class Alloc>
void allocator_t<Bits, Secure, Alloc>::detach(magnitude_t *p) {
    if (p->dec()) dealloc(p);
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_POLICY_HPP
