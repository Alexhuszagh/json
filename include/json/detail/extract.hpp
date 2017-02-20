//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup JSON
 *  \brief Extract data from encoded strings.
 */

#pragma once

#include "type.hpp"


namespace json
{
namespace detail
{
// OBJECTS
// -------


/** \brief Extract basic value from string.
 */
template <typename T>
struct Extract
{
    /** \brief Overload string.
     */
    template <typename U = T>
    enable_if_t<is_string_v<U>, const U&>
    operator()(const std::string &string)
    {
        return string;
    }

    /** \brief Overload char.
     */
    template <typename U = T>
    enable_if_t<is_char_v<U>, U>
    operator()(const std::string &string)
    {
        return string.empty() ? '\0' : string.front();
    }

    /** \brief Overload boolean.
     */
    template <typename U = T>
    enable_if_t<is_bool_v<U>, U>
    operator()(const std::string &string)
    {
        return string == "true";
    }

    /** \brief Overload integer.
     */
    template <typename U = T>
    enable_if_t<is_integer_v<U>, U>
    operator()(const std::string &string)
    {
        return std::stol(string);
    }

    /** \brief Overload float.
     */
    template <typename U = T>
    enable_if_t<is_float_v<U>, U>
    operator()(const std::string &string)
    {
        return std::stod(string);
    }
};

}   /* detail */
}   /* json */
