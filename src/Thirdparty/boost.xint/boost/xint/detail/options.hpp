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
    \brief Declares the integer_t template options.
*/

#ifndef BOOST_INCLUDED_XINT_INTEGER_OPTIONS_HPP
#define BOOST_INCLUDED_XINT_INTEGER_OPTIONS_HPP

namespace boost {
namespace xint {

#ifndef BOOST_XINT_DOXYGEN_IGNORE
BOOST_PARAMETER_TEMPLATE_KEYWORD(fixedlength_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(threadsafe_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(secure_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(nothrow_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(allocator_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(sign_tag)
#endif // BOOST_XINT_DOXYGEN_IGNORE

using boost::is_base_of;
using boost::mpl::_;
using boost::parameter::binding;
using boost::parameter::optional;
using boost::parameter::deduced;

#ifndef BOOST_XINT_DOXYGEN_IGNORE
namespace detail {
enum negative_policy {
    // This says it's signed, and is the default.
    p_signed_type,

    // This says it's unsigned, and to use the default negative policy (modulus
    // for fixed-length, throw_exception for variable-length).
    p_unsigned_type,

    // These say that it's unsigned, and to use the specific negative policy
    // named.
    p_modulus,
    p_throw_exception, // or return Not-a-Number for nothrow types
    p_force_zero,
    p_force_absolute
};
} // namespace detail
#endif // BOOST_XINT_DOXYGEN_IGNORE

/*! \brief Options for the integer_t template are stored in this namespace.

See the detailed description section for the
\ref boost::xint::integer_t "integer_t" class for an overview of these options
and their defaults.
*/
namespace options {

#ifndef BOOST_XINT_DOXYGEN_IGNORE
struct fixedlength_base { };
struct threadsafe_base { };
struct secure_base { };
struct nothrow_base { };
struct sign_base { };
#endif // BOOST_XINT_DOXYGEN_IGNORE

//! \name Fixed-Length Integers
//!@{
/*! \brief Makes fixed-length integers.

\tparam Bits The number of bits to limit it to, or zero for unlimited-length.

If \c Bits is non-zero, integers of this type are \ref fixedlength_type
"fixed-length", limited to the specified number of bits. If
zero (the default), the %integer is limited only by your system's memory.
See also the \ref boost::xint::options::negative_modulus "negative_modulus"
option.
*/
template <bitsize_t Bits> struct fixedlength: public fixedlength_base, public
    boost::mpl::integral_c<bitsize_t, Bits> { };
//!@}

//! \name Makes an integer type that is safe to use in multiple threads.
//!@{
/*! \brief Ensures that the integer_t objects can be safely used by multiple
           threads.

This is the opposite of the \ref
boost::xint::options::copy_on_write "copy_on_write" option. It is the default
behavior; the option is provided only to make it easier to write
self-documenting code.

Note that any number of threads can safely \b read an integer_t object
simultaneously, but if even one thread is \b writing to it, no other thread can
safely access it for any reason until the write is done, even with this option.
It is your responsibility to ensure that only the writing thread has access to
the object during the write.
*/
struct threadsafe: public threadsafe_base, public boost::mpl::bool_<true> { };

/*! \brief Allows faster operations, at a price.

This is the opposite of the \ref boost::xint::options::threadsafe "threadsafe"
option.

By default, the library ensures that each object has its own unique storage
before returning it. If this option is used, identical objects are allowed to
share storage using a copy-on-write design, potentially increasing the speed of
the library, but making it unsafe for multithreaded use (with \ref
threadsafe "some exceptions"). The library allows copy-on-write for internal
functions regardless of this parameter, which is safe because the internal
functions only use one thread at a time.
*/
struct copy_on_write: public threadsafe_base, public boost::mpl::bool_<false>{};
//!@}

//! \name Miscellaneous Options
//!@{
/*! \brief Zeros memory before deallocating.

If used, the library zeros out all memory before deallocating it, for maximum
security. The default is not to do so, for maximum speed.

\note
This provides only one half of a truly secure solution. Depending on how your
operating system handles virtual memory, numbers may be paged out to disk at any
time. To ensure maximum security, combine it with an allocator that prevents its
memory from being paged to disk under any circumstances.
*/
struct secure: public secure_base, public boost::mpl::bool_<true> { };

/*! \brief Disables exceptions for the type.

If used, then operations using this type do not throw exceptions, they return
special values (most often \ref nan "Not-a-Number") to indicate failures. The
default is to allow exceptions.
*/
struct nothrow: public nothrow_base, public boost::mpl::bool_<true> { };
//!@}

//! \name Signed/Unsigned Options
//!@{
/*! \brief Creates a signed integer type.

If none of the other \c negative_* options is specified, this one is used by
default. It can also be explicitly specified, to make self-documenting code
easier to write.
*/
struct negative_allowed: public sign_base, public
    boost::mpl::integral_c<detail::negative_policy, detail::p_signed_type>
    { };

/*! \brief Creates an unsigned integer with the most common negative-value
           handling.

This is a sort of default unsigned option. If the type is fixed-length, it acts
like \c negative_modulus; if not, it acts like \c negative_exception.
*/
struct negative_not_allowed: public sign_base, public
    boost::mpl::integral_c<detail::negative_policy, detail::p_unsigned_type>
    { };

/*! \brief Makes a fixedlength integer wrap like a built-in unsigned type.

This option can only be used with fixed-length types. If a calculation would
result in a negative number, the number is "wrapped" in the same way that the
built-in unsigned types are. For example, decrementing a zero will give you the
maximum value for the type.
*/
struct negative_modulus: public sign_base, public
    boost::mpl::integral_c<detail::negative_policy, detail::p_modulus>
    { };

/*! \brief Creates an integer type that considers negative values invalid.

When using this option, if a calculation would result in a negative number, the
library throws a \c cannot_represent exception. If exceptions are blocked, the
calculation results in a Not-a-Number value instead.
*/
struct negative_exception: public sign_base, public
    boost::mpl::integral_c<detail::negative_policy, detail::p_throw_exception>
    { };

/*! \brief Creates an integer type that forces any negative number to zero.

This is useful for saturation arithmetic, though you'll have to handle the upper
bound yourself.

\see http://en.wikipedia.org/wiki/Saturation_arithmetic
*/
struct negative_to_zero: public sign_base, public
    boost::mpl::integral_c<detail::negative_policy, detail::p_force_zero>
    { };

/*! \brief Creates an integer type that forces any negative number to its
           absolute value.

A negative result while using this option is automatically changed to its
positive counterpart.
*/
struct negative_absolute: public sign_base, public
    boost::mpl::integral_c<detail::negative_policy, detail::p_force_absolute>
    { };
//!@}

#ifndef BOOST_XINT_DOXYGEN_IGNORE
typedef parameter::parameters<
    optional<deduced<tag::fixedlength_tag>, is_base_of<fixedlength_base, _> >,
    optional<deduced<tag::threadsafe_tag>,  is_base_of<threadsafe_base, _> >,
    optional<deduced<tag::secure_tag>,      is_base_of<secure_base, _> >,
    optional<deduced<tag::nothrow_tag>,     is_base_of<nothrow_base, _> >,
    optional<deduced<tag::sign_tag>,        is_base_of<sign_base, _> >,
    optional<deduced<tag::allocator_tag>,   is_class<_> >
> integer_signature;
#endif

} // namespace options

namespace detail {

////////////////////////////////////////////////////////////////////////////////
// integer_t data

#ifdef BOOST_XINT_DOXYGEN_IGNORE
    // The documentation should see a simplified version of the template
    // parameters.
    #define BOOST_XINT_INITIAL_APARAMS ...
    #define BOOST_XINT_CLASS_APARAMS ...
    #define BOOST_XINT_CLASS_BPARAMS other
    #define BOOST_XINT_APARAMS ...
    #define BOOST_XINT_BPARAMS other
#else
    #define BOOST_XINT_INITIAL_APARAMS \
        class A0 = parameter::void_, \
        class A1 = parameter::void_, \
        class A2 = parameter::void_, \
        class A3 = parameter::void_, \
        class A4 = parameter::void_, \
        class A5 = parameter::void_
    #define BOOST_XINT_CLASS_APARAMS class A0, class A1, class A2, class A3, \
        class A4, class A5
    #define BOOST_XINT_APARAMS A0, A1, A2, A3, A4, A5
    #define BOOST_XINT_CLASS_BPARAMS class B0, class B1, class B2, class B3, \
        class B4, class B5
    #define BOOST_XINT_BPARAMS B0, B1, B2, B3, B4, B5
#endif

#ifndef BOOST_XINT_DOXYGEN_IGNORE

template<BOOST_XINT_CLASS_APARAMS>
class integer_t_data {
    public:
    typedef typename options::integer_signature::bind<BOOST_XINT_APARAMS>::type
        args;
    typedef const typename binding<args, tag::fixedlength_tag,
        options::fixedlength<0> >::type BitsType;
    typedef const typename binding<args, tag::threadsafe_tag, mpl::true_>::type
        ThreadsafeType;
    typedef const typename binding<args, tag::secure_tag, mpl::false_>::type
        SecureType;
    typedef const typename binding<args, tag::nothrow_tag, mpl::false_>::type
        NothrowType;
    typedef const typename binding<args, tag::sign_tag,
        options::negative_allowed>::type SignType;

    static const bitsize_t Bits;
    static const bool Threadsafe, Secure, Nothrow, Signed;
    typedef typename binding<args, tag::allocator_tag,
        std::allocator<detail::digit_t> >::type Alloc;
    typedef raw_integer_t<BitsType::value, SecureType::value, Alloc> datatype;

    integer_t_data() { }
    integer_t_data(const datatype& copy): data(copy) { }
    template <bitsize_t B, bool S, class A>
    integer_t_data(const raw_integer_t<B,S,A>& copy): data(copy) { }

    //! \name Internal Functions
    //@{
    datatype& _data() { return data; }
    const datatype& _data() const { return data; }
    std::size_t _get_length() const { return data.length; }
    detail::digit_t _get_digit(std::size_t i) const { return data[i]; }
    void _make_unique() { data.make_unique(); }
    //@}

    protected:
    datatype data;
};

template<BOOST_XINT_CLASS_APARAMS>
const bitsize_t integer_t_data<BOOST_XINT_APARAMS>::Bits =
    integer_t_data<BOOST_XINT_APARAMS>::BitsType::value;

template<BOOST_XINT_CLASS_APARAMS>
const bool integer_t_data<BOOST_XINT_APARAMS>::Threadsafe =
    integer_t_data<BOOST_XINT_APARAMS>::ThreadsafeType::value;

template<BOOST_XINT_CLASS_APARAMS>
const bool integer_t_data<BOOST_XINT_APARAMS>::Secure =
    integer_t_data<BOOST_XINT_APARAMS>::SecureType::value;

template<BOOST_XINT_CLASS_APARAMS>
const bool integer_t_data<BOOST_XINT_APARAMS>::Nothrow =
    integer_t_data<BOOST_XINT_APARAMS>::NothrowType::value;

template<BOOST_XINT_CLASS_APARAMS>
const bool integer_t_data<BOOST_XINT_APARAMS>::Signed =
    (integer_t_data<BOOST_XINT_APARAMS>::SignType::value == p_signed_type);

////////////////////////////////////////////////////////////////////////////////
// class nan_functions, to handle nothrow-specific Not-a-Number stuff

template <bool Nothrow, typename T, BOOST_XINT_CLASS_APARAMS>
class nan_functions { };

template <typename T, BOOST_XINT_CLASS_APARAMS>
class nan_functions<false, T, BOOST_XINT_APARAMS>: virtual public
    integer_t_data<BOOST_XINT_APARAMS>
{
    public:
    bool is_nan() const { return false; }
};

template <typename T, BOOST_XINT_CLASS_APARAMS>
class nan_functions<true, T, BOOST_XINT_APARAMS>: virtual public
    integer_t_data<BOOST_XINT_APARAMS>
{
    public:
    bool is_nan() const {
        return integer_t_data<BOOST_XINT_APARAMS>::data.is_nan();
    }

    static T nan() { return get_nan<T>(); }
};

////////////////////////////////////////////////////////////////////////////////
// class fixed_functions, to handle fixed-specific stuff

template <bitsize_t Bits, typename T, BOOST_XINT_CLASS_APARAMS>
class fixed_functions: virtual public integer_t_data<BOOST_XINT_APARAMS> {
    using integer_t_data<BOOST_XINT_APARAMS>::Threadsafe;
    using integer_t_data<BOOST_XINT_APARAMS>::Nothrow;
    using integer_t_data<BOOST_XINT_APARAMS>::Signed;

    public:
    T operator~() const {
        if (Nothrow) {
            if (integer_t_data<BOOST_XINT_APARAMS>::data.is_nan())
                return detail::get_nan<T>();
            BOOST_XINT_TRY {
                T r(~integer_t_data<BOOST_XINT_APARAMS>::data);
                if (Threadsafe == true) r._make_unique();
                if (!Signed) r.data.negative = false;
                else r.data.negative = !r.data.negative;
                return BOOST_XINT_MOVE(r);
            } BOOST_XINT_CATCH {
                return detail::get_nan<T>();
            }
        } else {
            T r(~integer_t_data<BOOST_XINT_APARAMS>::data);
            if (Threadsafe == true) r._make_unique();
            if (!Signed) r.data.negative = false;
            else r.data.negative = !r.data.negative;
            return BOOST_XINT_MOVE(r);
        }
    }
};

template <typename T, BOOST_XINT_CLASS_APARAMS>
class fixed_functions<0, T, BOOST_XINT_APARAMS>: virtual public
    integer_t_data<BOOST_XINT_APARAMS>
{
    // Nothing needed here at present.
};

////////////////////////////////////////////////////////////////////////////////
// class unsigned_negative_functions, for... well, unsigned negatives. :-)

template <negative_policy P, BOOST_XINT_CLASS_APARAMS>
class unsigned_negative_functions { };

template <BOOST_XINT_CLASS_APARAMS>
class unsigned_negative_functions<p_signed_type, BOOST_XINT_APARAMS> {
    public:
    void _fix_negative_unsigned() { } // Nothing to do for signed numbers
};

template <BOOST_XINT_CLASS_APARAMS>
class unsigned_negative_functions<p_unsigned_type, BOOST_XINT_APARAMS>: virtual
    public integer_t_data<BOOST_XINT_APARAMS>, public
    unsigned_negative_functions<(integer_t_data<BOOST_XINT_APARAMS>::BitsType::
    value != 0 ? p_modulus : p_throw_exception), BOOST_XINT_APARAMS>
{
    // Don't need anything here, it inherits from the proper default item
    // (modulus for fixed-length, throw_exception for variable-length)
};

template <BOOST_XINT_CLASS_APARAMS>
class unsigned_negative_functions<p_modulus, BOOST_XINT_APARAMS>: virtual
    public integer_t_data<BOOST_XINT_APARAMS>
{
    // Types using unsigned_modulus must be fixedlength too.
    typedef typename integer_t_data<BOOST_XINT_APARAMS>::BitsType BitsType;
    BOOST_STATIC_ASSERT(BitsType::value != 0);

    public:
    void _fix_negative_unsigned() {
        typename integer_t_data<BOOST_XINT_APARAMS>::datatype&
            data(integer_t_data<BOOST_XINT_APARAMS>::_data());
        data = ~data;
        ++data;
    }
};

template <BOOST_XINT_CLASS_APARAMS>
class unsigned_negative_functions<p_throw_exception, BOOST_XINT_APARAMS>:
    virtual public integer_t_data<BOOST_XINT_APARAMS>
{
    public:
    void _fix_negative_unsigned() {
        if (integer_t_data<BOOST_XINT_APARAMS>::NothrowType::value) {
            integer_t_data<BOOST_XINT_APARAMS>::_data().make_nan();
        } else {
            exception_handler<>::call(__FILE__, __LINE__,
                exceptions::cannot_represent("unsigned negative value"));
        }
    }
};

template <BOOST_XINT_CLASS_APARAMS>
class unsigned_negative_functions<p_force_zero, BOOST_XINT_APARAMS>: virtual
    public integer_t_data<BOOST_XINT_APARAMS>
{
    public:
    void _fix_negative_unsigned() {
        typename integer_t_data<BOOST_XINT_APARAMS>::datatype&
            data(integer_t_data<BOOST_XINT_APARAMS>::_data());
        data.set(0);
    }
};

template <BOOST_XINT_CLASS_APARAMS>
class unsigned_negative_functions<p_force_absolute, BOOST_XINT_APARAMS>: virtual
    public integer_t_data<BOOST_XINT_APARAMS>
{
    public:
    void _fix_negative_unsigned() {
        typename integer_t_data<BOOST_XINT_APARAMS>::datatype&
            data(integer_t_data<BOOST_XINT_APARAMS>::_data());
        data.negative = false;
    }
};

#endif // BOOST_XINT_DOXYGEN_IGNORE

} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_INTEGER_OPTIONS_HPP
