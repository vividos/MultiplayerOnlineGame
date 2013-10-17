
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
    \brief Declares the arbitrary-length integer type.
*/

#ifndef BOOST_INCLUDED_XINT_INTEGER_HPP
#define BOOST_INCLUDED_XINT_INTEGER_HPP

#ifdef _MSC_VER
    // C4127, "conditional expression is constant". Unfortunately the policy-
    //   based design of this library triggers a lot of these, even though the
    //   code is correct as written.
    #pragma warning(push)
    #pragma warning(disable: 4127)
#endif

#include "detail/internals.hpp"
#include "random.hpp"

namespace boost {
namespace xint {

/*! \brief The integer_t class template.

This class implements the aribitrary-length %integer type.

There are only a few member functions, primarily operators. Most of the
functionality is implemented using standalone functions.

You can specify several template parameters for the type. The defaults are:
- \ref boost::xint::options::threadsafe "threadsafe" (can be changed with
\ref boost::xint::options::copy_on_write "copy_on_write")
- variable-length (can be changed with \ref
boost::xint::options::fixedlength "fixedlength")
- \ref boost::xint::options::negative_allowed "negative_allowed" (can be changed
with \ref boost::xint::options::negative_absolute "negative_absolute",
\ref boost::xint::options::negative_exception "negative_exception",
\ref boost::xint::options::negative_modulus "negative_modulus", or
\ref boost::xint::options::negative_to_zero "negative_to_zero")
- throw exceptions on unrecoverable errors (can be changed with
\ref boost::xint::options::nothrow "nothrow")
- don't attempt to clear storage when destroying integer_t objects (can be
changed with \ref boost::xint::options::secure "secure")
- use the STL-provided allocator \c std::allocator<digit_t> as the allocator
type (can be changed by specifying a new allocator for the \c
boost::xint::detail::digit_t type)

Other than the last, which needs no helper class, all of these options are found
in the #boost::xint::options namespace. Through the magic of Boost.Parameter,
you can specify as few or as many of these options as you need, and in any
order.

Here are some examples of how to specify an integer type:

<code><pre>
    namespace xopts = boost::xint::options;
    typedef boost::xint::integer_t<xopts::fixedlength<512>, xopts::secure,
        xopts::negative_modulus> secure_uint512;
    typedef boost::xint::integer_t<my_allocator<boost::xint::detail::digit_t>,
        xopts::copy_on_write> my_integer;
</pre></code>

Note that the basic \ref boost::xint::integer "integer" and \ref
boost::xint::nothrow_integer "nothrow_integer" types are already typedef'd for
you.
*/
template<BOOST_XINT_INITIAL_APARAMS>
class integer_t: virtual public detail::integer_t_data<BOOST_XINT_APARAMS>,
    public detail::nan_functions<detail::integer_t_data<BOOST_XINT_APARAMS>::
    NothrowType::value, integer_t<BOOST_XINT_APARAMS>, BOOST_XINT_APARAMS>,
    public detail::fixed_functions<detail::integer_t_data<BOOST_XINT_APARAMS>::
    BitsType::value, integer_t<BOOST_XINT_APARAMS>, BOOST_XINT_APARAMS>, public
    detail::unsigned_negative_functions<detail::integer_t_data<
    BOOST_XINT_APARAMS>::SignType::value, BOOST_XINT_APARAMS>
{
    typedef typename detail::integer_t_data<BOOST_XINT_APARAMS>::BitsType
        BitsType;
    typedef typename detail::integer_t_data<BOOST_XINT_APARAMS>::SecureType
        SecureType;
    typedef typename detail::integer_t_data<BOOST_XINT_APARAMS>::SignType
        SignType;
    typedef typename detail::integer_t_data<BOOST_XINT_APARAMS>::Alloc Alloc;

    public:
    #ifndef BOOST_XINT_DOXYGEN_IGNORE
    typedef integer_t<BOOST_XINT_APARAMS> type;
    typedef void (integer_t<BOOST_XINT_APARAMS>::*safe_bool_type)() const;
    void this_is_a_safe_bool_type() const { }

    typedef typename detail::integer_t_data<BOOST_XINT_APARAMS>::datatype
        datatype;

    using detail::integer_t_data<BOOST_XINT_APARAMS>::Threadsafe;
    using detail::integer_t_data<BOOST_XINT_APARAMS>::Nothrow;
    using detail::integer_t_data<BOOST_XINT_APARAMS>::Signed;

    using detail::integer_t_data<BOOST_XINT_APARAMS>::data;
    using detail::integer_t_data<BOOST_XINT_APARAMS>::_data;

    using detail::nan_functions<detail::integer_t_data<
        BOOST_XINT_APARAMS>::NothrowType::value, integer_t<
        BOOST_XINT_APARAMS>, BOOST_XINT_APARAMS>::is_nan;

    using detail::unsigned_negative_functions<detail::integer_t_data<
        BOOST_XINT_APARAMS>::SignType::value, BOOST_XINT_APARAMS>::
        _fix_negative_unsigned;
    #endif // BOOST_XINT_DOXYGEN_IGNORE

    //! \name Constructors & Destructors
    //!@{
    integer_t();
    integer_t(const integer_t<BOOST_XINT_APARAMS> &b, bool force_thread_safety =
        false);
    //! \brief The move constructor.
    integer_t(BOOST_XINT_RV_REF(type) i) { _swap(i); }
    template <typename charT>
    explicit integer_t(const charT *str, std::size_t base = 10);
    template <typename charT>
    integer_t(const charT *str, const charT *&endptr, std::size_t base = 10);
    template <typename charT, typename traitsT, typename allocT>
    explicit integer_t(const std::basic_string<charT, traitsT, allocT> &str,
        std::size_t base = 10);
    integer_t(const xint::binary_t &b, bitsize_t bits = 0);
    template <BOOST_XINT_CLASS_BPARAMS> explicit integer_t(const
        integer_t<BOOST_XINT_BPARAMS> &other, bool force_thread_safety = false);
    template <typename Type> integer_t(const Type n, typename
        boost::enable_if<boost::is_integral<Type> >::type* = 0);

    #ifndef BOOST_XINT_DOXYGEN_IGNORE
    //! This is used internally.
    explicit integer_t(const detail::raw_integer_t<BitsType::value,
        SecureType::value, Alloc> &c):
        detail::integer_t_data<BOOST_XINT_APARAMS>(c) { if (Threadsafe == true)
        _data().make_unique(); }
    #endif // BOOST_XINT_DOXYGEN_IGNORE
    //!@}

    /*! \name Mathematical and Bitwise Operators
        \brief These operate exactly like their counterparts for the built-in
               %integer types.
    */
    //@{
    integer_t<BOOST_XINT_APARAMS>& operator=(BOOST_XINT_COPY_ASSIGN_REF(type)
        c);
    integer_t<BOOST_XINT_APARAMS>& operator=(BOOST_XINT_RV_REF(type) c) {
        _swap(c); return *this; }
    template <typename Type> integer_t<BOOST_XINT_APARAMS>& operator=(const Type
        n) { integer_t<BOOST_XINT_APARAMS> nn(n); _swap(nn); return *this; }

    operator safe_bool_type() const { return (data.is_zero() ? 0 :
        &integer_t<BOOST_XINT_APARAMS>::this_is_a_safe_bool_type); }
    bool operator!() const { return data.is_zero(); }
    integer_t<BOOST_XINT_APARAMS> operator-() const;
    integer_t<BOOST_XINT_APARAMS>& operator+() { return *this; }
    const integer_t<BOOST_XINT_APARAMS>& operator+() const { return *this; }

    #ifdef BOOST_XINT_DOXYGEN_IGNORE // inherited, only here for doxygen
    integer_t<BOOST_XINT_APARAMS> operator~() const;
    #endif

    integer_t<BOOST_XINT_APARAMS>& operator+=(const
        integer_t<BOOST_XINT_APARAMS> &b);
    integer_t<BOOST_XINT_APARAMS>& operator-=(const
        integer_t<BOOST_XINT_APARAMS> &b);
    integer_t<BOOST_XINT_APARAMS>& operator*=(const
        integer_t<BOOST_XINT_APARAMS> &b);
    integer_t<BOOST_XINT_APARAMS>& operator/=(const
        integer_t<BOOST_XINT_APARAMS> &b);
    integer_t<BOOST_XINT_APARAMS>& operator%=(const
        integer_t<BOOST_XINT_APARAMS> &b);

    integer_t<BOOST_XINT_APARAMS>& operator++();
    integer_t<BOOST_XINT_APARAMS>& operator--();
    integer_t<BOOST_XINT_APARAMS>  operator++(int);
    integer_t<BOOST_XINT_APARAMS>  operator--(int);

    integer_t<BOOST_XINT_APARAMS>& operator&=(const
        integer_t<BOOST_XINT_APARAMS> &n);
    integer_t<BOOST_XINT_APARAMS>& operator|=(const
        integer_t<BOOST_XINT_APARAMS> &n);
    integer_t<BOOST_XINT_APARAMS>& operator^=(const
        integer_t<BOOST_XINT_APARAMS> &n);
    integer_t<BOOST_XINT_APARAMS>  operator<<(bitsize_t shift) const;
    integer_t<BOOST_XINT_APARAMS>  operator>>(bitsize_t shift) const;
    integer_t<BOOST_XINT_APARAMS>& operator<<=(bitsize_t shift);
    integer_t<BOOST_XINT_APARAMS>& operator>>=(bitsize_t shift);
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool is_odd() const;
    bool is_even() const;
    int  sign(bool signed_zero = false) const;
    std::size_t hex_digits() const;

    #ifdef BOOST_XINT_DOXYGEN_IGNORE // inherited, only here for doxygen
    bool is_nan() const;
    #endif
    //!@}

    typedef base_divide_t<integer_t<BOOST_XINT_APARAMS> > divide_t;

    /*! \name Static Member Functions

        These are functions that return an integer_t, but don't take one as a
        parameter. To use them as free functions, you would have to specify the
        return type anyway, so they're static member functions of the type
        instead.
    */
    //!@{
    static integer_t<BOOST_XINT_APARAMS> pow2(std::size_t exponent);
    static integer_t<BOOST_XINT_APARAMS> factorial(std::size_t n);

    #ifdef BOOST_XINT_DOXYGEN_IGNORE // inherited, only here for doxygen
    static integer_t<BOOST_XINT_APARAMS> nan();
    #endif

    template <class Type> static integer_t<BOOST_XINT_APARAMS>
        random_by_size(Type &gen, bitsize_t size_in_bits, bool high_bit_on =
        false, bool low_bit_on = false, bool can_be_negative = false);
    template <class Type> static integer_t<BOOST_XINT_APARAMS>
        random_prime(Type &gen, bitsize_t size_in_bits, callback_t callback =
        no_callback);
    //!@}

    #ifndef BOOST_XINT_DOXYGEN_IGNORE
    //! \name Internal Functions
    //@{
    void _swap(integer_t<BOOST_XINT_APARAMS> &s) { using std::swap;
        swap(data, s.data); }
    //@}
    #endif

    private:
    BOOST_XINT_COPYABLE_AND_MOVABLE(type)
};

////////////////////////////////////////////////////////////////////////////////
// Member function template definitions

//! \brief Creates a new integer with an initial value of zero.
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>::integer_t() {
    // Don't need to do anything, already preinitialized to zero.
}

/*! \brief Creates a copy of an existing integer.

\param[in] i An existing integer.

\param[in] force_thread_safety If the \c threadsafe template parameter is \c
false, you can use this to make a thread-safe copy anyway. See \ref
threadsafe "this page" for a full treatment of the matter.

\overload
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>::integer_t(const integer_t<BOOST_XINT_APARAMS> &i,
    bool force_thread_safety)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            data = i.data;
            if (!data.is_nan()) {
                if (force_thread_safety || Threadsafe == true)
                    data.make_unique();
            }
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        data = i.data;
        if (force_thread_safety || Threadsafe == true) data.make_unique();
    }
}

//! \copydoc integer_t(const std::basic_string<charT, traitsT, allocT>&, std::size_t)
template<BOOST_XINT_CLASS_APARAMS>
template <typename charT>
integer_t<BOOST_XINT_APARAMS>::integer_t(const charT *str, std::size_t base) {
    if (Nothrow) {
        BOOST_XINT_TRY {
            const std::basic_string<charT> &tnan = detail::nan_text<charT>();
            if (str[0] == tnan[0] && std::basic_string<charT>(str) == tnan) {
                data.make_nan();
            } else {
                data.from_string(str, base);
                if (!Signed && data.negative) _fix_negative_unsigned();
            }
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        data.from_string(str, base);
        if (!Signed && data.negative) _fix_negative_unsigned();
    }
}

/*! \brief Parse an integer from a string representation.

- Complexity: O(n<sup>2</sup>)

\param[in] str A string representation of a number.
\param[out] endptr When the function returns, this will contain a pointer to the
first character after the parsed integer.
\param[in] base The base that the string representation of the number is in.
This can be any number between 2 and 36 (inclusive). It can also be the constant
xint::autobase, in which case the function will follow the standard C/C++ rules
for interpreting a numeric constant: any number with a zero as the first digit
is assumed to be base-8; any number with a leading zero-x or zero-X (such as
0x1f) is base-16, and anything else is base-10.

\exception exceptions::invalid_base if the base parameter is not between 2 and
36 (inclusive) or the constant xint::autobase.
\exception exceptions::invalid_digit if there are no valid digits before the
first invalid character.

\remarks
This function is based on the \c strtol function from the standard library. Like
\c strtol, it first discards any leading whitespace characters, then takes as
many characters as are valid for the given \c base and interprets them as an
integer. Then a pointer to the first character following the integer is stored
in the object pointed by \c endptr.

\overload
*/
template<BOOST_XINT_CLASS_APARAMS>
template <typename charT>
integer_t<BOOST_XINT_APARAMS>::integer_t(const charT *str, const charT *&endptr,
    std::size_t base)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            const std::basic_string<charT> &tnan = detail::nan_text<charT>();
            if (str[0] == tnan[0]) {
                std::basic_string<charT> s(str);
                if (s.substr(0, tnan.length()) == tnan) {
                    data.make_nan();
                    endptr = str + tnan.length();
                    return;
                }
            }
            data.from_string(str, endptr, base);
            if (!Signed && data.negative) _fix_negative_unsigned();
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        data.from_string(str, endptr, base);
        if (!Signed && data.negative) _fix_negative_unsigned();
    }
}

/*! \brief Create an integer from a string representation.

- Complexity: O(n<sup>2</sup>)

\param[in] str A string representation of a number.
\param[in] base The base that the string representation of the number is in.
This can be any number between 2 and 36 (inclusive). It can also be the constant
xint::autobase, in which case the function will follow the standard C/C++ rules
for interpreting a numeric constant: any number with a zero as the first digit
is assumed to be base-8; any number with a leading zero-x or zero-X (such as
0x1f) is base-16, and anything else is base-10.

\exception exceptions::invalid_base if the base parameter is not between 2 and
36 (inclusive) or the constant xint::autobase.
\exception exceptions::invalid_digit if the string contains any digit that
cannot be part of a number in the specified base, or if there are no valid
digits.

\remarks
This is the most common way to initialize literal values that are too large to
fit into a native integral type.

\overload
*/
template<BOOST_XINT_CLASS_APARAMS>
template <typename charT, typename traitsT, typename allocT>
integer_t<BOOST_XINT_APARAMS>::integer_t(const std::basic_string<charT, traitsT,
    allocT> &str, std::size_t base)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            if (str == detail::nan_text<charT>()) data.make_nan();
            else {
                data.from_string(str, base);
                if (!Signed && data.negative) _fix_negative_unsigned();
            }
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        data.from_string(str, base);
        if (!Signed && data.negative) _fix_negative_unsigned();
    }
}

/*! \brief Converts a portable binary representation of a number into an
           %integer.

- Complexity: O(n)

\param[in] i An \c xint::binary_t containing the bytes to convert, lowest byte
first.
\param[in] bits The number of bits packed into each character (for portability
with systems that might not have characters with the same number of bits), or
zero (the default) to assume the number of bits in the current system's
unsigned character type.

\exception exceptions::invalid_argument if \c bits is greater than the number of
bits in an unsigned character.

\returns An %integer representing the bytes.

\see xint::to_binary

\overload
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>::integer_t(const xint::binary_t &i, bitsize_t
    bits)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            data.from_binary(i, bits);
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        data.from_binary(i, bits);
    }
}

/*! Constructs a new integer object from an existing integer object.

Converts any existing integer_t object to this type.

\param[in] i An existing integer.

\param[in] force_thread_safety If the \c threadsafe template parameter is \c
false, you can use this to make a thread-safe copy anyway. See \ref
threadsafe "this page" for a full treatment of the matter.

\overload
*/
template<BOOST_XINT_CLASS_APARAMS>
template <BOOST_XINT_CLASS_BPARAMS>
integer_t<BOOST_XINT_APARAMS>::integer_t(const integer_t<BOOST_XINT_BPARAMS> &i,
    bool force_thread_safety)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            if (i.is_nan()) {
                data.make_nan();
            } else {
                data = i._data();
                if (!data.is_nan()) {
                    if (force_thread_safety || Threadsafe == true)
                        data.make_unique();
                    if (!Signed && data.negative) _fix_negative_unsigned();
                }
            }
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        if (i._data().is_nan()) detail::exception_handler<>::call(__FILE__,
            __LINE__, exceptions::not_a_number());
        data = i._data();
        if (force_thread_safety || Threadsafe == true) data.make_unique();
        if (!Signed && data.negative) _fix_negative_unsigned();
    }
}

/*!
    Constructs a new integer object with the value of the built-in %integer type
    passed to it.

    \param[in] n The value for the new object.
    \tparam Type The type of \c n.

    You can ignore the \c enable_if parameter, it's there only to tell the
    compiler what types the function applies to.

    \overload
*/
template<BOOST_XINT_CLASS_APARAMS>
template <typename Type> integer_t<BOOST_XINT_APARAMS>::integer_t(const Type n,
    typename boost::enable_if<boost::is_integral<Type> >::type*)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            if (std::numeric_limits<Type>::is_signed) {
                data.set_signed(n);
                if (!Signed && data.negative) _fix_negative_unsigned();
            } else data.set_unsigned(n);
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        if (std::numeric_limits<Type>::is_signed) {
            data.set_signed(n);
            if (!Signed && data.negative) _fix_negative_unsigned();
        } else data.set_unsigned(n);
    }
}

template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::
    operator=(BOOST_XINT_COPY_ASSIGN_REF(type) c)
{
    if (Nothrow) {
        if (c._data().is_nan()) data.make_nan();
        else {
            BOOST_XINT_TRY {
                data = c.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data = c.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

#ifdef BOOST_XINT_DOXYGEN_IGNORE // Doesn't really exist, this is here for docs
/*! \brief The Bitwise Ones Complement (i.e. bitwise NOT) operator.

- Complexity: O(n)

\note
This operator is only available with fixed-size integers. The result from a
proper implementation for variable-sized integers would be an infinitely long
number.

\par
Also note that, although this changes the bit-pattern the same way as with the
built-in types, the \e value will not be the same for signed numbers, because
this library does not store negative numbers in twos complement format. In
other words, if \c n is a built-in signed type and \c m is a signed integer_t
with the same value, then <code>~n != ~m</code>.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> integer_t<BOOST_XINT_APARAMS>::operator~() const;
#endif // BOOST_XINT_DOXYGEN_IGNORE

/*! \brief Return the additive inverse of an integer.

- Complexity: O(1)

\returns \c *this with the sign reversed.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> integer_t<BOOST_XINT_APARAMS>::operator-() const {
    if (Nothrow) {
        if (is_nan()) return *this;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(*this);
            if (Threadsafe == true) r._make_unique();
            r.data = -r.data;
            if (!Signed && r.data.negative) r._fix_negative_unsigned();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(*this);
        if (Threadsafe == true) r._make_unique();
        r.data = -r.data;
        if (!Signed && r.data.negative) r._fix_negative_unsigned();
        return BOOST_XINT_MOVE(r);
    }
}

//! \see operator+(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator+=(const
    integer_t<BOOST_XINT_APARAMS> &b)
{
    if (Nothrow) {
        if (b.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data += b.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data += b.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

//! \see operator-(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator-=(const
    integer_t<BOOST_XINT_APARAMS> &b)
{
    if (Nothrow) {
        if (b.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data -= b.data;
                if (Threadsafe == true) data.make_unique();
                if (!Signed && data.negative) _fix_negative_unsigned();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data -= b.data;
        if (Threadsafe == true) data.make_unique();
        if (!Signed && data.negative) _fix_negative_unsigned();
    }
    return *this;
}

//! \see operator*(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator*=(const
    integer_t<BOOST_XINT_APARAMS> &b)
{
    if (Nothrow) {
        if (b.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data *= b.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data *= b.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

//! \see operator/(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator/=(const
    integer_t<BOOST_XINT_APARAMS> &b)
{
    if (Nothrow) {
        if (b.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data /= b.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data /= b.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

//! \see operator%(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator%=(const
    integer_t<BOOST_XINT_APARAMS> &b)
{
    if (Nothrow) {
        if (b.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data %= b.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data %= b.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

/*! Preincrement operator

- Complexity: amortized O(1)
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator++() {
    if (Nothrow) {
        if (!is_nan()) {
            BOOST_XINT_TRY {
                ++data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        ++data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

/*! Predecrement operator

- Complexity: amortized O(1)
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator--() {
    if (Nothrow) {
        if (!is_nan()) {
            BOOST_XINT_TRY {
                --data;
                if (Threadsafe == true) data.make_unique();
                if (!Signed && data.negative) _fix_negative_unsigned();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        --data;
        if (Threadsafe == true) data.make_unique();
        if (!Signed && data.negative) _fix_negative_unsigned();
    }
    return *this;
}

/*! Postincrement operator

- Complexity: O(n)

This is not recommended. It must make a copy before incrementing the \c *this
object, making it noticeably less efficient than the preincrement operator.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> integer_t<BOOST_XINT_APARAMS>::operator++(int) {
    if (Nothrow) {
        if (is_nan()) return *this;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(data++);
            if (Threadsafe == true) { r._make_unique(); data.make_unique(); }
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(data++);
        if (Threadsafe == true) { r._make_unique(); data.make_unique(); }
        return BOOST_XINT_MOVE(r);
    }
}

/*! Postdecrement operator

- Complexity: O(n)

This is not recommended. It must make a copy before decrementing the \c *this
object, making it noticeably less efficient than the predecrement operator.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> integer_t<BOOST_XINT_APARAMS>::operator--(int) {
    if (Nothrow) {
        if (is_nan()) return *this;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(data--);
            if (Threadsafe == true) { r._make_unique(); data.make_unique(); }
            if (!Signed && data.negative) _fix_negative_unsigned();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            data.make_nan();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(data--);
        if (Threadsafe == true) { r._make_unique(); data.make_unique(); }
        if (!Signed && data.negative) _fix_negative_unsigned();
        return BOOST_XINT_MOVE(r);
    }
}

//! \see operator&(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator&=(const
    integer_t<BOOST_XINT_APARAMS> &n)
{
    if (Nothrow) {
        if (n.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data &= n.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data &= n.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

//! \see operator|(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator|=(const
    integer_t<BOOST_XINT_APARAMS> &n)
{
    if (Nothrow) {
        if (n.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data |= n.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data |= n.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

//! \see operator^(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>& integer_t<BOOST_XINT_APARAMS>::operator^=(const
    integer_t<BOOST_XINT_APARAMS> &n)
{
    if (Nothrow) {
        if (n.is_nan()) data.make_nan();
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data ^= n.data;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data ^= n.data;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

/*! \brief Left-shift an integer by a specified number of bits.

- Complexity: O(n)

\param[in] shift The number of bits to left-shift the number.

\returns The bit-shifted integer.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>
integer_t<BOOST_XINT_APARAMS>::operator<<(bitsize_t shift) const {
    if (Nothrow) {
        if (is_nan()) return *this;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(data << shift);
            if (Threadsafe == true) r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(data << shift);
        if (Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Right-shift an integer by a specified number of bits.

- Complexity: O(n)

\param[in] shift The number of bits to right-shift the number.

\returns The bit-shifted integer.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>
integer_t<BOOST_XINT_APARAMS>::operator>>(bitsize_t shift) const {
    if (Nothrow) {
        if (is_nan()) return *this;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(data >> shift);
            if (Threadsafe == true) r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(data >> shift);
        if (Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

//! \see operator<<(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>&
integer_t<BOOST_XINT_APARAMS>::operator<<=(bitsize_t shift) {
    if (Nothrow) {
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data <<= shift;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data <<= shift;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

//! \see operator>>(integer_t, integer_t)
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>&
integer_t<BOOST_XINT_APARAMS>::operator>>=(bitsize_t shift) {
    if (Nothrow) {
        if (!is_nan()) {
            BOOST_XINT_TRY {
                data >>= shift;
                if (Threadsafe == true) data.make_unique();
            } BOOST_XINT_CATCH {
                data.make_nan();
            }
        }
    } else {
        data >>= shift;
        if (Threadsafe == true) data.make_unique();
    }
    return *this;
}

/*! \brief Tests the lowest bit of \c *this to determine oddness.

- Complexity: O(1)

\returns \c true if \c *this is odd, otherwise \c false. The \ref
exceptions "nothrow version" returns false instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
bool integer_t<BOOST_XINT_APARAMS>::is_odd() const {
    if (Nothrow && is_nan()) return false;
    return data.is_odd();
}

/*! \brief Tests the lowest bit of \c *this to determine evenness.

- Complexity: O(1)

\returns \c true if \c *this is even, otherwise \c false. The \ref
exceptions "nothrow version" returns false instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
bool integer_t<BOOST_XINT_APARAMS>::is_even() const {
    if (Nothrow && is_nan()) return false;
    return data.is_even();
}

#ifdef BOOST_XINT_DOXYGEN_IGNORE // Doesn't really exist, this is here for docs
/*! \brief Tests whether the object is a Not-a-Number.

- Complexity: O(1)

\returns \c true if \c *this is a Not-a-Number value, otherwise \c false.

\see \ref nan

\note This will always return \c false on any type where the \c nothrow template
parameter is \c false.
*/
template<BOOST_XINT_CLASS_APARAMS>
bool integer_t<BOOST_XINT_APARAMS>::is_nan() const;
#endif // BOOST_XINT_DOXYGEN_IGNORE

/*! \brief Tests the sign of \c *this.

- Complexity: O(1)

\param[in] signed_zero If \c false (the default value), returns zero if \c *this
is zero. If \c true, returns 1 or -1 on a zero \c *this as well. Primarily used
to identify a \ref zero "negative zero".

\returns -1 if \c *this is negative, 0 if it's zero, or 1 if it's greater than
zero. The \ref exceptions "nothrow version" returns zero instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
int integer_t<BOOST_XINT_APARAMS>::sign(bool signed_zero) const {
    if (Nothrow) {
        if (is_nan()) return 0;
        BOOST_XINT_TRY {
            return data.sign(signed_zero);
        } BOOST_XINT_CATCH {
            return 0;
        }
    } else {
        return data.sign(signed_zero);
    }
}

/*! \brief Tells you roughly how large \c *this is.

- Complexity: O(1)

\returns The number of hexadecimal digits that would be required to encode \c
*this. The \ref exceptions "nothrow version" returns zero instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
size_t integer_t<BOOST_XINT_APARAMS>::hex_digits() const {
    if (Nothrow) {
        if (is_nan()) return 0;
        BOOST_XINT_TRY {
            return data.hex_digits();
        } BOOST_XINT_CATCH {
            return 0;
        }
    } else {
        return data.hex_digits();
    }
}

/*! \brief Calculate the value of 2<sup>e</sup>

- Complexity: O(1)

\param[in] exponent The exponent to operate on.

\returns 2 to the power of \c exponent.

\remarks
This is a convenience function, to help with self-documenting code. It is also
more efficient than using bit-shifting or the \c pow function to get the same
result.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> integer_t<BOOST_XINT_APARAMS>::pow2(std::size_t
    exponent)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r;
            detail::pow2(r.data, exponent);
            if (Threadsafe == true) r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r;
        detail::pow2(r.data, exponent);
        if (Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate the value of \c n!

- Complexity: O(n<sup>3</sup>)

\param[in] n The value to operate on.

\returns \c n factorial, defined as <code>1*2*3*...*n</code>

\warning
Factorials get ridiculously huge, even with fairly small values of \c n. This
function, when used on an unlimited-size integer and with a large parameter, is
the easiest way to exhaust the system's memory.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS>
integer_t<BOOST_XINT_APARAMS>::factorial(std::size_t n) {
    if (Nothrow) {
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r;
            detail::factorial(r.data, n);
            if (Threadsafe == true) r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r;
        detail::factorial(r.data, n);
        if (Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

#ifdef BOOST_XINT_DOXYGEN_IGNORE // Doesn't really exist, this is here for docs
/*! \brief Returns a Not-a-Number value, for types that support it.

- Complexity: O(1)

\returns A Not-a-Number value of the specified type.

\note
This function only exists on types with the \c nothrow template parameter.

\see \ref nan
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> integer_t<BOOST_XINT_APARAMS>::nan();
#endif // BOOST_XINT_DOXYGEN_IGNORE

/*! \brief Generates a random integer with specific attributes.

- Complexity: O(n)

\param[in] gen The random number generator to use. This can be
boost::xint::default_random_generator, boost::xint::strong_random_generator, or
any other class that supplies the member functions required by the Boost.Random
random number generators.
\param[in] size_in_bits The maximum number of bits that you want the returned
number to have.
\param[in] high_bit_on If \c true, the returned number will have exactly the
requested size. If \c false, the upper bits may be zero, resulting in a number
that is slightly smaller than requested.
\param[in] low_bit_on If \c true, the returned number will always be odd. If
\c false, it has an equal chance of being odd or even.
\param[in] can_be_negative If \c true, the returned value has an equal chance
of being positive or negative. If \c false, it will always be positive. Note
that on unsigned types, this parameter is ignored; the result will always be
positive regardless of it.

\returns A random integer with the requested attributes.

\see \ref random
*/
template<BOOST_XINT_CLASS_APARAMS>
template <class Type>
integer_t<BOOST_XINT_APARAMS>integer_t<BOOST_XINT_APARAMS>::random_by_size(Type
    &gen, bitsize_t size_in_bits, bool high_bit_on, bool low_bit_on, bool
    can_be_negative)
{
    if (!Signed) can_be_negative = false;
    if (Nothrow) {
        BOOST_XINT_TRY {
            return integer_t<BOOST_XINT_APARAMS>(datatype::random_by_size(gen,
                size_in_bits, high_bit_on, low_bit_on, can_be_negative));
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        return integer_t<BOOST_XINT_APARAMS>(datatype::random_by_size(gen,
            size_in_bits, high_bit_on, low_bit_on, can_be_negative));
    }
}

/*! \brief Generate a randomly-generated prime number of a particular bit-size.

- Complexity: Varies

\param[in] gen The random number generator to use. This can be
boost::xint::default_random_generator, boost::xint::strong_random_generator, or
any other class that supplies the member functions required by the Boost.Random
random number generators.
\param[in] size_in_bits The number of bits that you want the returned value to
have.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns A randomly-generated prime integer with the specified number of bits,
or zero if the provided callback function cancelled the operation.

\exception exceptions::invalid_argument if \c size_in_bits is less than two.

\note This function uses xint::is_prime. See the description of it for details
of its limitations.

\par
See the \link random Random Number Functions page\endlink for details on how to
get cryptographically-secure random numbers.

\see \ref ex_primes
*/
template<BOOST_XINT_CLASS_APARAMS>
template <class Type>
integer_t<BOOST_XINT_APARAMS> integer_t<BOOST_XINT_APARAMS>::random_prime(Type
    &gen, bitsize_t size_in_bits, callback_t callback)
{
    if (Nothrow) {
        BOOST_XINT_TRY {
            return integer_t<BOOST_XINT_APARAMS>(datatype::random_prime(gen,
                size_in_bits, callback));
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        return integer_t<BOOST_XINT_APARAMS>(datatype::random_prime(gen,
            size_in_bits, callback));
    }
}

////////////////////////////////////////////////////////////////////////////////
// Free function template definitions
/*! \name Mathematical primitives

    There are only a few functions here because almost all mathematical
    primitives are implemented as operators.
*/
//!@{

/*! \brief Returns the absolute value of an integer.

- Complexity: O(1)

\param[in] n The integer to operate on.

\returns If \c n is zero or positive, returns \c n. Otherwise returns \c -n.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> abs(const integer_t<BOOST_XINT_APARAMS> &n) {
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n._data().abs());
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n._data().abs());
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate how many \c dividends would fit into \c divisor, with the
           remainder.

- Complexity: O(n<sup>2</sup>)

\param[in] dividend, divisor The integers to operate on.

\returns A \c divide_t class containing the quotient and remainder of \c
dividend divided by \c divisor. The nothrow version returns a \c divide_t with
both items set to Not-a-Number instead of throwing.

\exception exceptions::divide_by_zero if \c d2 is zero.

\see operator/(integer_t, integer_t)
\see operator%(integer_t, integer_t)
\see integer_t::operator/=
\see integer_t::operator%=
*/
template<BOOST_XINT_CLASS_APARAMS>
typename integer_t<BOOST_XINT_APARAMS>::divide_t divide(const
    integer_t<BOOST_XINT_APARAMS> &dividend, const integer_t<BOOST_XINT_APARAMS>
    &divisor)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (dividend.is_nan() || divisor.is_nan()) return typename
            integer_t<BOOST_XINT_APARAMS>::divide_t(detail::get_nan<
                integer_t<BOOST_XINT_APARAMS> >(), detail::get_nan<
                integer_t<BOOST_XINT_APARAMS> >());
        BOOST_XINT_TRY {
            typename integer_t<BOOST_XINT_APARAMS>::datatype::divide_t rr =
                detail::divide(dividend._data(), divisor._data());
            typename integer_t<BOOST_XINT_APARAMS>::divide_t
                r(integer_t<BOOST_XINT_APARAMS>(rr.quotient),
                integer_t<BOOST_XINT_APARAMS>(rr.remainder));
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return typename integer_t<BOOST_XINT_APARAMS>::divide_t(
                detail::get_nan<integer_t<BOOST_XINT_APARAMS> >(),
                detail::get_nan<integer_t<BOOST_XINT_APARAMS> >());
        }
    } else {
        typename integer_t<BOOST_XINT_APARAMS>::datatype::divide_t rr =
            detail::divide(dividend._data(), divisor._data());
        typename integer_t<BOOST_XINT_APARAMS>::divide_t
            r(integer_t<BOOST_XINT_APARAMS>(rr.quotient),
            integer_t<BOOST_XINT_APARAMS>(rr.remainder));
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate the absolute difference between two integers.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns The absolute difference between the parameters.

This function is especially useful when using unsigned types, because it's
faster than doing a comparison to see which integer is larger before doing a
subtraction.
*/
template <BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> difference(const integer_t<BOOST_XINT_APARAMS>
    &n1, const integer_t<BOOST_XINT_APARAMS> &n2)
{
    typename integer_t<BOOST_XINT_APARAMS>::datatype rdata = n1._data() -
        n2._data();
    rdata.negative = false;
    return integer_t<BOOST_XINT_APARAMS>(rdata);
}
//!@}

/*! \name Powers and roots

    \see integer_t::pow2
    \see integer_t::factorial
*/
//!@{

/*! \brief Calculate the value of n<sup>2</sup>

- Complexity: O(n(n+1)/2)

\param[in] n The integer to operate on.

\returns \c n times \c n.

The repetitive nature of the input (multiplying a number by itself) allows this
function to use a more-efficient algorithm than standard multiplication.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> square(const integer_t<BOOST_XINT_APARAMS> &n) {
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return n;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(detail::square(n._data()));
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(detail::square(n._data()));
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate the value of n<sup>e</sup>

- Complexity: O(n<sub>n</sub><sup>2</sup> 4<sup>n<sub>e</sub></sup>)

\param[in] n, e The integers to operate on.

\returns \c n to the power of \c e.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> pow(const integer_t<BOOST_XINT_APARAMS> &n, const
    integer_t<BOOST_XINT_APARAMS> &e)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan() || e.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r;
            pow(r._data(), n._data(), e._data());
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r;
        pow(r._data(), n._data(), e._data());
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate the square root of \c an integer.

- Complexity: O(log(n)*n<sup>2</sup>)

Uses Newton iteration with an initial guess of 2<sup>floor(log<sub>2</sub>(n)/2)
</sup>

\param[in] n The value to operate on.

\returns The largest integer that, when squared, is less than or equal to \c n.
If \c n is not a perfect square, then this value will not be the exact square
root.

\exception exceptions::cannot_represent if \c n is negative.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> square_root(const integer_t<BOOST_XINT_APARAMS>
    &n)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return n;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(square_root(n._data()));
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(square_root(n._data()));
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}
//!@}

/*! \name Conversion functions

    Note that these only convert an %integer type to something else. See the
    %integer constructors for conversions the other way.
*/
//!@{

/*! \brief Converts an xint::integer type to a built-in %integer type.

\param[in] n The %integer to convert.
\tparam T The type to convert it to.

\returns The numeric value of \c n, converted to the specified type.

\exception exceptions::too_big if \c n would not fit into the specified type.

This function provides the most efficient means of converting to a built-in
integral type. The nothrow version returns T(0) instead of throwing.
*/
template <typename T, BOOST_XINT_CLASS_APARAMS>
T to(const integer_t<BOOST_XINT_APARAMS> &n) {
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return T(0);
        BOOST_XINT_TRY {
            return to<T>(n._data());
        } BOOST_XINT_CATCH {
            return T(0);
        }
    } else {
        return to<T>(n._data());
    }
}

/*! \brief Creates a string representation of the specified %integer.

- Complexity: O(n<sup>2</sup>)

\tparam charT The type of string to create.

\param[in] n The %integer to convert.
\param[in] base The base, between 2 and 36 inclusive, to convert it to. Defaults
to base 10.
\param[in] uppercase Whether to make alphabetic characters (for bases greater
than ten) uppercase or not. Defaults to \c false.

\returns The string value of \c n. The nothrow version returns the string
"#NaN#" if \c n is a Not-a-Number, and an empty string instead of throwing.

\exception exceptions::invalid_base if base is less than two or greater than 36.

\remarks
This is the function that's called when you ask the library to write an %integer
to a stream, but it's more flexible because you can specify any base between 2
and 36. (Streams only allow base-8, base-10, or base-16.)
*/
template<typename charT, BOOST_XINT_CLASS_APARAMS>
std::basic_string<charT> to_stringtype(const integer_t<BOOST_XINT_APARAMS> &n,
    std::size_t base = 10, bool uppercase = false)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return detail::nan_text<charT>();
        BOOST_XINT_TRY {
            return detail::to_string<charT>(n._data(), base, uppercase);
        } BOOST_XINT_CATCH {
            return std::basic_string<charT>();
        }
    } else {
        return detail::to_string<charT>(n._data(), base, uppercase);
    }
}

/*! \brief A shorthand function that calls \ref
           integer_t::to_stringtype() "to_stringtype<char>".
*/
template<BOOST_XINT_CLASS_APARAMS>
std::string to_string(const integer_t<BOOST_XINT_APARAMS> &n, std::size_t base =
    10, bool uppercase = false)
{
    return to_stringtype<char>(n, base, uppercase);
}

/*! \brief A shorthand function that calls \ref
           integer_t::to_stringtype() "to_stringtype<wchar_t>".
*/
template<BOOST_XINT_CLASS_APARAMS>
std::wstring to_wstring(const integer_t<BOOST_XINT_APARAMS> n, std::size_t base
    = 10, bool uppercase = false)
{
    return to_stringtype<wchar_t>(n, base, uppercase);
}

/*! \brief Creates a binary representation of an %integer.

- Complexity: O(n)

\param[in] n The %integer to convert.
\param[in] bits The number of bits to pack into each character (for portability
with systems that might not have characters with the same number of bits), or
zero (the default) to assume the number of bits in the current system's
unsigned character type.

\returns An \c xint::binary_t containing the binary representation, lowest byte
first. The nothrow version returns an empty \c binary_t object instead of
throwing.

\exception exceptions::invalid_argument if \c bits is greater than the number of
bits in an unsigned character.

\note
This function only stores the absolute value of \c n; if you need the sign, you
must store it separately.

\remarks
A binary representation is sometimes desirable for persistent storage or
transmission, as it is more space-efficient than a string representation.

\see integer_t::integer_t(xint::binary_t, bitsize_t bits)
*/
template<BOOST_XINT_CLASS_APARAMS>
xint::binary_t to_binary(const integer_t<BOOST_XINT_APARAMS> &n, bitsize_t bits
    = 0)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return xint::binary_t();
        BOOST_XINT_TRY {
            return to_binary(n._data(), bits);
        } BOOST_XINT_CATCH {
            return xint::binary_t();
        }
    } else {
        return to_binary(n._data(), bits);
    }
}
//!@}

//! \name Bit-manipulation functions
//!@{

/*! \brief Queries the value of a particular bit in an integer.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] bit The zero-based index of the bit you're asking about.

\returns \c true if the specified bit is set (has a value of one), \c false if
it is clear. The nothrow version returns \c false instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
bool getbit(const integer_t<BOOST_XINT_APARAMS> &n, bitsize_t bit) {
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return false;
        BOOST_XINT_TRY {
            return getbit(n._data(), bit);
        } BOOST_XINT_CATCH {
            return false;
        }
    } else {
        return getbit(n._data(), bit);
    }
}

/*! \brief Sets a specific bit in an integer to one.

- Complexity: O(1)

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
template<BOOST_XINT_CLASS_APARAMS>
void setbit(integer_t<BOOST_XINT_APARAMS> &n, bitsize_t bit) {
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (!n.is_nan()) {
            BOOST_XINT_TRY {
                setbit(n._data(), bit);
            } BOOST_XINT_CATCH {
                // Do nothing.
            }
        }
    } else {
        setbit(n._data(), bit);
    }
}

/*! \brief Sets a specific bit in an integer to zero.

- Complexity: O(1)

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
template<BOOST_XINT_CLASS_APARAMS>
void clearbit(integer_t<BOOST_XINT_APARAMS> &n, bitsize_t bit) {
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (!n.is_nan()) {
            BOOST_XINT_TRY {
                clearbit(n._data(), bit);
            } BOOST_XINT_CATCH {
                // Do nothing.
            }
        }
    } else {
        clearbit(n._data(), bit);
    }
}

/*! \brief Obtains the index of the lowest bit in an integer with a value of
           one.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] return_if_zero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the lowest one-bit in the integer, or \c
return_if_zero if the integer contains no set bits. The nothrow version
returns \c return_if_zero instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
bitsize_t lowestbit(const integer_t<BOOST_XINT_APARAMS> &n, bitsize_t
    return_if_zero = 0)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return return_if_zero;
        BOOST_XINT_TRY {
            return lowestbit(n._data(), return_if_zero);
        } BOOST_XINT_CATCH {
            return return_if_zero;
        }
    } else {
        return lowestbit(n._data(), return_if_zero);
    }
}

/*! \brief Obtains the index of the highest bit in an integer with a value of
           one.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] return_if_zero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the highest one-bit in the integer, or \c
return_if_zero if the integer contains no set bits. The nothrow version returns
\c return_if_zero instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
bitsize_t highestbit(const integer_t<BOOST_XINT_APARAMS> &n, bitsize_t
    return_if_zero = 0)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return return_if_zero;
        BOOST_XINT_TRY {
            return highestbit(n._data(), return_if_zero);
        } BOOST_XINT_CATCH {
            return return_if_zero;
        }
    } else {
        return highestbit(n._data(), return_if_zero);
    }
}
//!@}

/*! \name Modular math functions

    A common reason to use a large-integer library is to implement public-key
    encryption, and the algorithms for such encryption often use modular math.

    \see operator%(integer_t, integer_t)
    \see integer_t::operator%=
*/
//!@{

/*! \brief Get the modular product of two integers.

- Complexity: O(2n<sup>2</sup>)

\param[in] n, by The integers to multiply.
\param[in] modulus The modulus.

\returns The result of \c n * \c by % \c modulus.

\remarks
This is purely a convenience function, to make it easier to write
self-documenting code. It does not provide any additional efficiency over
writing out the calculation.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> mulmod(const integer_t<BOOST_XINT_APARAMS> &n,
    const integer_t<BOOST_XINT_APARAMS> &by, const integer_t<BOOST_XINT_APARAMS>
    &modulus)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan() || by.is_nan() || modulus.is_nan()) return
            detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(mulmod(n._data(), by._data(),
                modulus._data()));
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(mulmod(n._data(), by._data(),
            modulus._data()));
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Get the modular product of squaring an integer.

- Complexity: O(2n<sup>2</sup>)

\param[in] n The integer to square.
\param[in] modulus The modulus.

\returns The result of \c square(n) % \c modulus.

\remarks
This is purely a convenience function, to make it easier to write
self-documenting code. It does not provide any additional efficiency over
writing out the calculation.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> sqrmod(const integer_t<BOOST_XINT_APARAMS> &n,
    const integer_t<BOOST_XINT_APARAMS> &modulus)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return n;
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r = sqrmod(n._data(),
                modulus._data());
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r = sqrmod(n._data(), modulus._data());
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Get the result of \c n<sup>exponent</sup> % \c modulus, keeping the
           intermediate results (relatively) small.

- Complexity: O(n<sub>n</sub><sup>2</sup>n<sub>e</sub>)

\param[in] n The integer to exponentiate.
\param[in] exponent The exponent.
\param[in] modulus The modulus.
\param[in] no_monty Used for testing, leave false for normal operation.

\returns The result of \c pow(n, \c exponent) % \c modulus.

\exception exceptions::invalid_modulus if the modulus is less than one.

\remarks
Because this function keeps the intermediate results small, it is far faster
and uses far less memory than a naive implementation.

\par
In addition, this function will use the Montgomery Reduction internally if the
modulus is an odd number (and if \c no_monty isn't set), which is almost always
faster than the non-Montgomery method.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> powmod(const integer_t<BOOST_XINT_APARAMS> &n,
    const integer_t<BOOST_XINT_APARAMS> &exponent, const
    integer_t<BOOST_XINT_APARAMS> &modulus, bool no_monty = false)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan() || exponent.is_nan() || modulus.is_nan()) return
            detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(powmod(n._data(), exponent._data(),
                modulus._data(), no_monty));
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(powmod(n._data(), exponent._data(),
            modulus._data(), no_monty));
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Get the modular inverse of a number in a modulus, if there is one.

- Complexity: O((log n)<sup>3</sup>)

\param[in] n The number to calculate the inverse of.
\param[in] modulus The modulus to use.

\returns The modular inverse of \c n in \c modulus. If \c n has no modular
inverse in \c modulus, returns zero.

\exception exceptions::invalid_modulus if the modulus is less than one.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> invmod(const integer_t<BOOST_XINT_APARAMS> &n,
    const integer_t<BOOST_XINT_APARAMS> &modulus)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan() || modulus.is_nan()) return
            detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(invmod(n._data(), modulus._data()));
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(invmod(n._data(), modulus._data()));
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}
//!@}

/*! \name Random and prime number functions

    \see integer_t::random_by_size
    \see integer_t::random_prime
*/
//!@{

/*! \brief Tests an integer for primality.

- Complexity: Varies

\param[in] n The number to test.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns 1 if \c n seems to be prime, 0 if it isn't, or -1 if the provided
callback function cancelled the operation. The nothrow version returns -2
instead of throwing.

\exception exceptions::invalid_argument if \c n is less than 2.

\remarks
This function uses the Rabin-Miller probabilistic primality test. There is an
infinitesimally small chance that it will report that a composite number is
actually prime, but that is so small that it can be ignored for most practical
purposes.

\par
Applied Cryptography, Second Edition, page 260 states that "for a 256-bit n, the
odds of error in six tests are less than 1 in 2^51", and the odds get slimmer
the larger the number is. (On the previous page, it says of 2^50 that "[...] if
you consider that the odds of the number being composite are 300 million times
less than the odds of winning top prize in a state lottery, you might not worry
about it so much.") That reference also recommends only five tests, which is
what the library uses.

\par
If even that chance is too much, you can run this function over the number
several times, it will use different randomly-selected "witness" numbers each
time.

\note If you wish to specify the source of the "witness" numbers, this function
has an overload where you can specify the random number generator to use.

\see \ref ex_primes
*/
template<BOOST_XINT_CLASS_APARAMS>
int is_probably_prime(const integer_t<BOOST_XINT_APARAMS> &n, callback_t
    callback = no_callback)
{
    typedef unsigned int rtype;
    typedef boost::mt19937 enginetype;
    typedef boost::uniform_int<rtype> dist_t;
    typedef boost::variate_generator<enginetype&, dist_t> gen_t;
    static std::auto_ptr<enginetype> engine(new
        enginetype(boost::uint32_t(time(0) + clock())));
    static std::auto_ptr<gen_t> gen(new gen_t(*engine.get(),
        dist_t((std::numeric_limits<rtype>::min)(),
            (std::numeric_limits<rtype>::max)())));
    return is_probably_prime(n, *gen.get(), callback);
}

/*! \brief Tests an integer for primality.

- Complexity: Varies

\param[in] n The number to test.
\param[in] generator The random number generator to use.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns 1 if \c n seems to be prime, 0 if it isn't, or -1 if the provided
callback function cancelled the operation. The nothrow version returns -2
instead of throwing.

\exception exceptions::invalid_argument if \c n is less than 2.

\remarks
This function uses the Rabin-Miller probabilistic primality test. There is an
infinitesimally small chance that it will report that a composite number is
actually prime, but that is so small that it can be ignored for most practical
purposes.

\par
Applied Cryptography, Second Edition, page 260 states that "for a 256-bit n, the
odds of error in six tests are less than 1 in 2^51", and the odds get slimmer
the larger the number is. (On the previous page, it says of 2^50 that "[...] if
you consider that the odds of the number being composite are 300 million times
less than the odds of winning top prize in a state lottery, you might not worry
about it so much.") That reference also recommends only five tests, which is
what the library uses.

\par
If even that chance is too much, you can run this function over the number
several times, it will use different randomly-selected "witness" numbers each
time.

\note If you don't care about the source of the witness numbers, this function
has an overload that supplies a default one.

\see \ref ex_primes
*/
template<BOOST_XINT_CLASS_APARAMS, typename GenT>
int is_probably_prime(const integer_t<BOOST_XINT_APARAMS> &n, GenT &generator,
    callback_t callback = no_callback)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return -2;
        BOOST_XINT_TRY {
            return is_probably_prime(n._data(), generator, callback);
        } BOOST_XINT_CATCH {
            return -2;
        }
    } else {
        return is_probably_prime(n._data(), generator, callback);
    }
}
//!@}

/*! \name Comparison Operators

    The nothrow versions of these functions return \c false instead of throwing.

    \see compare(integer_t, integer_t, bool)
*/
//!@{
template<BOOST_XINT_CLASS_APARAMS> bool operator<(const
    integer_t<BOOST_XINT_APARAMS> &n1, const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan()) return false;
        BOOST_XINT_TRY {
            return operator<(n1._data(), n2._data());
        } BOOST_XINT_CATCH {
            return false;
        }
    } else {
        return operator<(n1._data(), n2._data());
    }
}

template<BOOST_XINT_CLASS_APARAMS> bool operator>(const
    integer_t<BOOST_XINT_APARAMS> &n1, const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan()) return false;
        BOOST_XINT_TRY {
            return operator>(n1._data(), n2._data());
        } BOOST_XINT_CATCH {
            return false;
        }
    } else {
        return operator>(n1._data(), n2._data());
    }
}

template<BOOST_XINT_CLASS_APARAMS> bool operator<=(const
    integer_t<BOOST_XINT_APARAMS> &n1, const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan()) return false;
        BOOST_XINT_TRY {
            return operator<=(n1._data(), n2._data());
        } BOOST_XINT_CATCH {
            return false;
        }
    } else {
        return operator<=(n1._data(), n2._data());
    }
}

template<BOOST_XINT_CLASS_APARAMS> bool operator>=(const
    integer_t<BOOST_XINT_APARAMS> &n1, const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan()) return false;
        BOOST_XINT_TRY {
            return operator>=(n1._data(), n2._data());
        } BOOST_XINT_CATCH {
            return false;
        }
    } else {
        return operator>=(n1._data(), n2._data());
    }
}

template<BOOST_XINT_CLASS_APARAMS> bool operator==(const
    integer_t<BOOST_XINT_APARAMS> &n1, const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan()) return false;
        BOOST_XINT_TRY {
            return operator==(n1._data(), n2._data());
        } BOOST_XINT_CATCH {
            return false;
        }
    } else {
        return operator==(n1._data(), n2._data());
    }
}

template<BOOST_XINT_CLASS_APARAMS> bool operator!=(const
    integer_t<BOOST_XINT_APARAMS> &n1, const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan()) return false;
        BOOST_XINT_TRY {
            return operator!=(n1._data(), n2._data());
        } BOOST_XINT_CATCH {
            return false;
        }
    } else {
        return operator!=(n1._data(), n2._data());
    }
}
//!@}

/*! \name Mathematical and Bitwise Operators

    These act exactly the same as for the built-in %integer types.
*/
//!@{

/*! \brief Calculate the sum of two integers.

- Complexity: O(n)

\param[in] n1, n2 The integers to add.

\returns The sum of the parameters.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator+(const integer_t<BOOST_XINT_APARAMS> &n1,
    const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n1._data() + n2._data());
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n1._data() + n2._data());
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate the difference between two integers.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns The difference between the parameters.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator-(const integer_t<BOOST_XINT_APARAMS> &n1,
    const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n1._data() - n2._data());
            if (!integer_t<BOOST_XINT_APARAMS>::Signed && r._data().negative)
                r._fix_negative_unsigned();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n1._data() - n2._data());
        if (!integer_t<BOOST_XINT_APARAMS>::Signed && r._data().negative)
            r._fix_negative_unsigned();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate the product of two integers.

- Complexity: O(n<sup>2</sup>)

\param[in] n1, n2 The integers to operate on.

\returns The product of the parameters.

\remarks
Automatically uses the more-efficient squaring algorithm if it can trivially
detect that the two parameters are copies of the same number.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator*(const integer_t<BOOST_XINT_APARAMS> &n1,
    const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n1._data() * n2._data());
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n1._data() * n2._data());
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate how many \c dividends would fit into \c divisor.

- Complexity: O(n<sup>2</sup>)

\param[in] dividend, divisor The integers to operate on.

\returns The integer value of \c dividend divided by \c divisor.

\exception exceptions::divide_by_zero if \c divisor is zero.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator/(const integer_t<BOOST_XINT_APARAMS>
    &dividend, const integer_t<BOOST_XINT_APARAMS> &divisor)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (dividend.is_nan() || divisor.is_nan()) return
            detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(dividend._data() / divisor._data());
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(dividend._data() / divisor._data());
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Get the remainder after dividing a number by another.

- Complexity: O(n<sup>2</sup>)

\param[in] n1 The dividend.
\param[in] n2 The divisor.

\returns The remainder after dividing \c n1 by \c n2.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator%(const integer_t<BOOST_XINT_APARAMS> &n1,
    const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n1._data() % n2._data());
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n1._data() % n2._data());
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Combine two integers using a bitwise \c AND operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in both parameters turned
on.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator&(const integer_t<BOOST_XINT_APARAMS> &n1,
    const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n1._data() & n2._data());
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n1._data() & n2._data());
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Combine two integers using a bitwise \c OR operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in either parameter
turned on.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator|(const integer_t<BOOST_XINT_APARAMS> &n1,
    const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n1._data() | n2._data());
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n1._data() | n2._data());
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Combine two integers using a bitwise \c XOR operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in either parameter, but
not both, turned on.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> operator^(const integer_t<BOOST_XINT_APARAMS> &n1,
    const integer_t<BOOST_XINT_APARAMS> &n2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan())
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r(n1._data() ^ n2._data());
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r(n1._data() ^ n2._data());
        return BOOST_XINT_MOVE(r);
    }
}
//!@}

//! \name Miscellaneous functions
//!@{

/*! \brief Calculate the Greatest Common Denominator of two integers.

- Complexity: O((log n)<sup>3</sup>)

\param[in] num1, num2 The integers to operate on.

\returns The greatest common denominator of the two integers, which will always
be a positive number.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> gcd(const integer_t<BOOST_XINT_APARAMS> &num1,
    const integer_t<BOOST_XINT_APARAMS> &num2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (num1.is_nan() || num2.is_nan()) return
            detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r;
            gcd(r._data(), num1._data(), num2._data());
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r;
        gcd(r._data(), num1._data(), num2._data());
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Calculate the Least Common Multiple of two integers.

- Complexity: O((log n)<sup>3</sup> + n<sup>2</sup>)

\param[in] num1, num2 The integers to operate on.

\returns The least common multiple of the two integers. If either integer is
zero, then the return value will be zero, by convention; in all other cases, the
return value will be a positive number.
*/
template<BOOST_XINT_CLASS_APARAMS>
integer_t<BOOST_XINT_APARAMS> lcm(const integer_t<BOOST_XINT_APARAMS> &num1,
    const integer_t<BOOST_XINT_APARAMS> &num2)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (num1.is_nan() || num2.is_nan()) return
            detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        BOOST_XINT_TRY {
            integer_t<BOOST_XINT_APARAMS> r;
            lcm(r._data(), num1._data(), num2._data());
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true)
                r._make_unique();
            return BOOST_XINT_MOVE(r);
        } BOOST_XINT_CATCH {
            return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
        }
    } else {
        integer_t<BOOST_XINT_APARAMS> r;
        lcm(r._data(), num1._data(), num2._data());
        if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) r._make_unique();
        return BOOST_XINT_MOVE(r);
    }
}

/*! \brief Compare two integer objects.

- Complexity: varies between O(n) if the numbers are equal (the worst case),
down to O(1) if they have different signs or wildly different magnitudes.

\param[in] n1, n2 The integers to compare.
\param[in] ignoresign If \c true, the absolute values of n1 and n2 are compared,
instead of their signed values. Used internally.

\returns -1 if \c n1 < \c n2; zero if \c n1 == \c n2, or 1 if \c n1 > \c n2. The
nothrow version returns 2 instead of throwing.
*/
template<BOOST_XINT_CLASS_APARAMS>
int compare(const integer_t<BOOST_XINT_APARAMS> &n1, const
    integer_t<BOOST_XINT_APARAMS> &n2, bool ignoresign = false)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n1.is_nan() || n2.is_nan()) return 2;
        BOOST_XINT_TRY {
            return compare(n1._data(), n2._data(), ignoresign);
        } BOOST_XINT_CATCH {
            return 2;
        }
    } else {
        return compare(n1._data(), n2._data(), ignoresign);
    }
}

/*! \brief Get the log<sub>2</sub> value of an integer.

- Complexity: O(1)

\param[in] n The %integer to operate on.

\returns The %integer log<sub>2</sub> value of the integer. The nothrow version
returns zero instead of throwing.

\remarks
pow2(log2(n)-1) will give you an integer with the highest set bit of \c n,
assuming that \c n is non-zero.

\par
Similar to the #highestbit function.
*/
template<BOOST_XINT_CLASS_APARAMS>
std::size_t integer_log2(const integer_t<BOOST_XINT_APARAMS> &n) {
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        if (n.is_nan()) return 0;
        BOOST_XINT_TRY {
            return log2(n._data());
        } BOOST_XINT_CATCH {
            return 0;
        }
    } else {
        return log2(n._data());
    }
}
//!@}

//! Allows for comparisons between integer_t types and other integral values.
#define BOOST_XINT_ANY_COMPARE(rtype, op, sub) \
    template <BOOST_XINT_CLASS_APARAMS, typename N> \
    rtype sub(const integer_t<BOOST_XINT_APARAMS> &n1, const N n2, typename \
        boost::enable_if<boost::is_integral<N> >::type* = 0) \
    { \
        if (integer_t<BOOST_XINT_APARAMS>::Nothrow) { \
            BOOST_XINT_TRY { \
                return op(n1, integer_t<BOOST_XINT_APARAMS>(n2)); \
            } BOOST_XINT_CATCH { \
                return rtype(0); \
            } \
        } else { \
            return op(n1, integer_t<BOOST_XINT_APARAMS>(n2)); \
        } \
    } \
    \
    template <BOOST_XINT_CLASS_APARAMS, typename N> \
    rtype op(const integer_t<BOOST_XINT_APARAMS> &n1, const N n2) { \
        return BOOST_XINT_MOVE(sub(n1, n2)); \
    } \
    \
    template <typename N, BOOST_XINT_CLASS_APARAMS> \
    rtype sub(const N n1, const integer_t<BOOST_XINT_APARAMS> &n2, typename \
        boost::enable_if<boost::is_integral<N> >::type* = 0) \
    { \
        if (integer_t<BOOST_XINT_APARAMS>::Nothrow) { \
            BOOST_XINT_TRY { \
                return op(integer_t<BOOST_XINT_APARAMS>(n1), n2); \
            } BOOST_XINT_CATCH { \
                return rtype(0); \
            } \
        } else { \
            return op(integer_t<BOOST_XINT_APARAMS>(n1), n2); \
        } \
    } \
    \
    template <typename N, BOOST_XINT_CLASS_APARAMS> \
    rtype op(const N n1, const integer_t<BOOST_XINT_APARAMS> &n2) { \
        return BOOST_XINT_MOVE(sub(n1, n2)); \
    }

BOOST_XINT_ANY_COMPARE(bool, operator<, _sublt)
BOOST_XINT_ANY_COMPARE(bool, operator>, _subgt)
BOOST_XINT_ANY_COMPARE(bool, operator<=, _suble)
BOOST_XINT_ANY_COMPARE(bool, operator>=, _subge)
BOOST_XINT_ANY_COMPARE(bool, operator==, _subeq)
BOOST_XINT_ANY_COMPARE(bool, operator!=, _subne)
BOOST_XINT_ANY_COMPARE(int, compare, _subcompare)

//! Allows for operations between integer_t types and other integral values.
#define BOOST_XINT_ANY_MATH(op, sub) \
    template <BOOST_XINT_CLASS_APARAMS, typename N> \
    integer_t<BOOST_XINT_APARAMS> sub(const integer_t<BOOST_XINT_APARAMS> &n1, \
        const N n2, typename boost::enable_if<boost::is_integral<N> >::type* = \
        0) \
    { \
        if (integer_t<BOOST_XINT_APARAMS>::Nothrow) { \
            BOOST_XINT_TRY { \
                integer_t<BOOST_XINT_APARAMS> r(op(n1, \
                    integer_t<BOOST_XINT_APARAMS>(n2))); \
                if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) \
                    r._make_unique(); \
                if (!integer_t<BOOST_XINT_APARAMS>::Signed && \
                    r._data().negative) r._fix_negative_unsigned(); \
                return BOOST_XINT_MOVE(r); \
            } BOOST_XINT_CATCH { \
                return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >(); \
            } \
        } else { \
            integer_t<BOOST_XINT_APARAMS> r(op(n1, \
                integer_t<BOOST_XINT_APARAMS>(n2))); \
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) \
                r._make_unique(); \
            if (!integer_t<BOOST_XINT_APARAMS>::Signed && r._data().negative) \
                r._fix_negative_unsigned(); \
            return BOOST_XINT_MOVE(r); \
        } \
    } \
    \
    template <BOOST_XINT_CLASS_APARAMS, typename N> \
    integer_t<BOOST_XINT_APARAMS> op(const integer_t<BOOST_XINT_APARAMS> &n1, \
        const N n2) \
    { \
        return BOOST_XINT_MOVE(sub(n1, n2)); \
    } \
    \
    template <typename N, BOOST_XINT_CLASS_APARAMS> \
    integer_t<BOOST_XINT_APARAMS> sub(const N n1, const \
        integer_t<BOOST_XINT_APARAMS> &n2, typename \
        boost::enable_if<boost::is_integral<N> >::type* = 0) \
    { \
        if (integer_t<BOOST_XINT_APARAMS>::Nothrow) { \
            BOOST_XINT_TRY { \
                integer_t<BOOST_XINT_APARAMS> \
                    r(op(integer_t<BOOST_XINT_APARAMS>(n1), n2)); \
                if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) \
                    r._make_unique(); \
                if (!integer_t<BOOST_XINT_APARAMS>::Signed && \
                    r._data().negative) r._fix_negative_unsigned(); \
                return BOOST_XINT_MOVE(r); \
            } BOOST_XINT_CATCH { \
                return detail::get_nan<integer_t<BOOST_XINT_APARAMS> >(); \
            } \
        } else { \
            integer_t<BOOST_XINT_APARAMS> \
                r(op(integer_t<BOOST_XINT_APARAMS>(n1), n2)); \
            if (integer_t<BOOST_XINT_APARAMS>::Threadsafe == true) \
                r._make_unique(); \
            if (!integer_t<BOOST_XINT_APARAMS>::Signed && r._data().negative) \
                r._fix_negative_unsigned(); \
            return BOOST_XINT_MOVE(r); \
        } \
    } \
    \
    template <typename N, BOOST_XINT_CLASS_APARAMS> \
    integer_t<BOOST_XINT_APARAMS> op(const N n1, const \
        integer_t<BOOST_XINT_APARAMS> &n2) \
    { \
        return BOOST_XINT_MOVE(sub(n1, n2)); \
    }

BOOST_XINT_ANY_MATH(pow, _subpow)
BOOST_XINT_ANY_MATH(mulmod, _submulmod)
BOOST_XINT_ANY_MATH(sqrmod, _subsqrmod)
BOOST_XINT_ANY_MATH(powmod, _subpowmod)
BOOST_XINT_ANY_MATH(invmod, _subinvmod)
BOOST_XINT_ANY_MATH(operator+, _subadd)
BOOST_XINT_ANY_MATH(operator-, _subsubtract)
BOOST_XINT_ANY_MATH(operator*, _submultiply)
BOOST_XINT_ANY_MATH(operator/, _subdivide)
BOOST_XINT_ANY_MATH(operator%, _submod)
BOOST_XINT_ANY_MATH(operator&, _subbitand)
BOOST_XINT_ANY_MATH(operator|, _subbitor)
BOOST_XINT_ANY_MATH(operator^, _subbitxor)
BOOST_XINT_ANY_MATH(gcd, _subgcd)
BOOST_XINT_ANY_MATH(lcm, _sublcm)

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits, BOOST_XINT_CLASS_APARAMS> inline
    std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits> &out, const integer_t<BOOST_XINT_APARAMS> &n)
{
    if (integer_t<BOOST_XINT_APARAMS>::Nothrow && n.is_nan()) return
        operator<<(out, detail::nan_text<charT>());
    return operator<<(out, n._data());
}

template <typename charT, typename traits, BOOST_XINT_CLASS_APARAMS> inline
    std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits> &in, integer_t<BOOST_XINT_APARAMS> &n)
{
    //if (in.flags() &

    if (integer_t<BOOST_XINT_APARAMS>::Nothrow) {
        const std::basic_string<charT> &tnan = detail::nan_text<charT>();
        charT nextchar = charT(in.peek());
        if (nextchar == tnan[0]) {
            // Must be either #NaN# or an error
            std::vector<charT> buffer;

            charT p = 0;
            for (std::size_t i = 0; i < tnan.length(); ++i) {
                in >> p;
                buffer.push_back(p);
            }
            buffer.push_back(0);

            std::basic_string<charT> str(&buffer[0]);
            if (str == tnan)
                n = detail::get_nan<integer_t<BOOST_XINT_APARAMS> >();
            else in.setstate(std::ios::failbit);
            return in;
        }
    }
    operator>>(in, n._data());
    if (!integer_t<BOOST_XINT_APARAMS>::Signed && n._data().negative)
        n._fix_negative_unsigned();
    return in;
}
//!@}

template<BOOST_XINT_CLASS_APARAMS>
inline void swap(integer_t<BOOST_XINT_APARAMS> &left,
    integer_t<BOOST_XINT_APARAMS> &right)
{
    left._swap(right);
}

/*! \brief The default arbitrary-length integer type.

    This uses the default parameters for the integer_t template.
*/
typedef integer_t<> integer;

/*! \brief The default arbitrary-length nothrow_integer type.

    This uses the default parameters for the integer_t template, other than the
    \c nothrow one.
*/
typedef integer_t<options::nothrow> nothrow_integer;

} // namespace xint
} // namespace boost

#ifndef BOOST_XINT_DOXYGEN_IGNORE
namespace std {

template<BOOST_XINT_CLASS_APARAMS>
class numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> > {
    public:
    static const bool is_specialized;

    static boost::xint::integer_t<BOOST_XINT_APARAMS> min
        BOOST_PREVENT_MACRO_SUBSTITUTION() throw()
    {
        if (boost::xint::integer_t<BOOST_XINT_APARAMS>::Bits)
            return -~(boost::xint::integer_t<BOOST_XINT_APARAMS>());
        else return 0;
    }
    static boost::xint::integer_t<BOOST_XINT_APARAMS> max
        BOOST_PREVENT_MACRO_SUBSTITUTION() throw()
    {
        if (boost::xint::integer_t<BOOST_XINT_APARAMS>::Bits)
            return ~(boost::xint::integer_t<BOOST_XINT_APARAMS>());
        else return 0;
    }

    static const int digits;
    static const int digits10;
    static const bool is_signed;
    static const bool is_integer;
    static const bool is_exact;
    static const int radix;
    static boost::xint::integer_t<BOOST_XINT_APARAMS> epsilon() throw() { return
        0; }
    static boost::xint::integer_t<BOOST_XINT_APARAMS> round_error() throw() {
        return 0; }

    static const int min_exponent; // N/A
    static const int min_exponent10; // N/A
    static const int max_exponent; // N/A
    static const int max_exponent10; // N/A

    static const bool has_infinity; // N/A
    static const bool has_quiet_NaN;
    static const bool has_signaling_NaN; // N/A
    static const float_denorm_style has_denorm; // N/A
    static const bool has_denorm_loss; // N/A

    static boost::xint::integer_t<BOOST_XINT_APARAMS> infinity() throw() {
        return 0; } // N/A
    static boost::xint::integer_t<BOOST_XINT_APARAMS> quiet_NaN() throw() {
        if (boost::xint::integer_t<BOOST_XINT_APARAMS>::Nothrow) return boost::
            xint::detail::get_nan<boost::xint::integer_t<BOOST_XINT_APARAMS> >();
        else return 0;
    }
    static boost::xint::integer_t<BOOST_XINT_APARAMS> signaling_NaN() throw() {
        return 0; } // N/A
    static boost::xint::integer_t<BOOST_XINT_APARAMS> denorm_min() throw() {
        return 0; } // N/A

    static const bool is_iec559; // N/A
    static const bool is_bounded;
    static const bool is_modulo;

    static const bool traps; // I assume this means exceptions?
    static const bool tinyness_before; // N/A
    static const float_round_style round_style; // N/A
};

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    is_specialized = true;

template<BOOST_XINT_CLASS_APARAMS>
const int numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::digits =
    static_cast<int>(boost::xint::integer_t<BOOST_XINT_APARAMS>::Bits);

template<BOOST_XINT_CLASS_APARAMS>
const int numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::digits10
    = static_cast<int>(boost::xint::detail::log10_bits(
    boost::xint::integer_t<BOOST_XINT_APARAMS>::Bits));

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    is_signed = (boost::xint::integer_t<BOOST_XINT_APARAMS>::Signed ? true :
    false);

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    is_integer = true;

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::is_exact
    = true;

template<BOOST_XINT_CLASS_APARAMS>
const int numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::radix =
    2;

template<BOOST_XINT_CLASS_APARAMS>
const int numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    min_exponent = 0;

template<BOOST_XINT_CLASS_APARAMS>
const int numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    min_exponent10 = 0;

template<BOOST_XINT_CLASS_APARAMS>
const int numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    max_exponent = 0;

template<BOOST_XINT_CLASS_APARAMS>
const int numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    max_exponent10 = 0;

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    has_infinity = false;

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    has_quiet_NaN = (boost::xint::integer_t<BOOST_XINT_APARAMS>::Nothrow ? true
    : false);

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    has_signaling_NaN = false;

template<BOOST_XINT_CLASS_APARAMS>
const float_denorm_style numeric_limits<boost::xint::integer_t<
    BOOST_XINT_APARAMS> >::has_denorm = denorm_absent;

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    has_denorm_loss = false;

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    is_iec559 = false;

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    is_bounded = (boost::xint::integer_t<BOOST_XINT_APARAMS>::Bits == 0 ? false
    : true);

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    is_modulo = (boost::xint::integer_t<BOOST_XINT_APARAMS>::Bits == 0 ? false :
    true);

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::traps =
    (boost::xint::integer_t<BOOST_XINT_APARAMS>::Nothrow ? false : true);

template<BOOST_XINT_CLASS_APARAMS>
const bool numeric_limits<boost::xint::integer_t<BOOST_XINT_APARAMS> >::
    tinyness_before = false;

template<BOOST_XINT_CLASS_APARAMS>
const float_round_style numeric_limits<boost::xint::integer_t<
    BOOST_XINT_APARAMS> >::round_style = round_indeterminate;

} // namespace std

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif // BOOST_XINT_DOXYGEN_IGNORE
#endif // BOOST_INCLUDED_XINT_INTEGER_HPP
