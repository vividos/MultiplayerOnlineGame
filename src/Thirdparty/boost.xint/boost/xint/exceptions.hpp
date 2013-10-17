
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
    \brief The exception namespace, and exception-related functions.
*/

#ifndef BOOST_INCLUDED_XINT_EXCEPTIONS_HPP
#define BOOST_INCLUDED_XINT_EXCEPTIONS_HPP

#include <stdexcept>
#include "detail/basic_types_and_includes.hpp"

namespace boost {
namespace xint {

/*! \brief Sets or resets the global exception handler for the library.

\param fn The function to set as the exception handler, of type \ref
on_exception_t. If not specified, removes any previous exception handler.

\note
This function, and the underlying exception handler system, is \e not
thread-safe. It is your responsibility to ensure that no other threads are using
the library while calling this function.

\see \ref exception_handler
*/
inline void on_exception(on_exception_t fn = on_exception_t()) {
    detail::exception_handler<>::fn = fn;
}

//! \brief Every exception the library throws is from this namespace.
namespace exceptions {

/*!
    Derived from \c std::invalid_argument.
*/
class invalid_argument: public std::invalid_argument {
    public:
    invalid_argument(): std::invalid_argument("invalid argument") { }
    invalid_argument(const std::string &what): std::invalid_argument(what) { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class invalid_base: public std::invalid_argument {
    public:
    invalid_base(): invalid_argument("invalid base") { }
    invalid_base(const std::string &what): invalid_argument(what) { }
};

/*!
    Derived from \c std::range_error.
*/
class invalid_digit: public std::range_error {
    public:
    invalid_digit(): range_error("invalid digit") { }
    invalid_digit(const std::string &what): range_error(what) { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class invalid_modulus: public std::invalid_argument {
    public:
    invalid_modulus(): invalid_argument("invalid modulus") { }
    invalid_modulus(const std::string &what): invalid_argument(what) { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class divide_by_zero: public std::invalid_argument {
    public:
    divide_by_zero(): invalid_argument("divide by zero error") { }
    divide_by_zero(const std::string &what): invalid_argument(what) { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class cannot_represent: public std::invalid_argument {
    public:
    cannot_represent(): invalid_argument("cannot represent the requested value")
        { }
    cannot_represent(const std::string &what): invalid_argument(what) { }
};

/*!
    Derived from \c std::range_error.
*/
class too_big: public std::range_error {
    public:
    too_big(): range_error("value out of range for requested conversion") { }
    too_big(const std::string &what): range_error(what) { }
};

/*!
    Derived from \c std::runtime_error.
*/
class not_a_number: public std::runtime_error {
    public:
    not_a_number(): runtime_error("attempted to use a Not-a-Number") { }
    not_a_number(const std::string &what): runtime_error(what) { }
};

/*!
    Derived from \c std::runtime_error.

    \remarks
    Thrown by the xint::strong_random_generator class's constructor, if the
    system does not provide a cryptographically-secure random generator. Also
    thrown if the generator fails when asked to provide a random value, though
    this should be vanishingly rare.
*/
class no_strong_random: public std::runtime_error {
    public:
    no_strong_random(): runtime_error("system does not have a strong random "
        "generator") { }
    no_strong_random(const std::string &what): runtime_error(what) { }
};

/*!
    Derived from \c std::overflow_error.

    \remarks
    Thrown when the library can't get enough memory to represent a number.
*/
class overflow_error: public std::overflow_error {
    public:
    overflow_error(): std::overflow_error("overflow error: can't obtain enough "
        "memory to represent the requested number") { }
    overflow_error(const std::string &what): std::overflow_error(what) { }
};

} // namespace exception
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_EXCEPTIONS_HPP
