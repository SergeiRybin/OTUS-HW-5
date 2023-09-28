#pragma once
/**
* @file
* @author Sergei Rybin <sergei.rybin@softeq.com>
* @version 1.0
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* @section DESCRIPTION
*
* The sfinae file shows abilities of SFINAE approach in C++.
*/
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <cassert>

template <typename T>
struct is_container : std::false_type
{
};

template <typename U>
struct is_container<std::vector<U>> : std::true_type
{
};

template <typename U>
struct is_container<std::list<U>> : std::true_type
{
};

/**
 * @brief Print function specialization for integral type.
 * @tparam T  integral type. SFINAE will reject other ones.
 * @return The string representing IP as a set of octets delimited by dots.
 *
 * @details
 * This function makes the IP as a string from the input.
 */
template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::string> print_ip(const T &t)
{
    std::ostringstream ret;
    for (int i = sizeof(T) - 1; i >= 0; --i)
    {
        unsigned char c = t >> i * 8;
        ret << std::to_string(c);
        if (i > 0)
        {
            ret << ".";
        }
    }
    return ret.str();
}

/**
 * @brief Print function specialization for std::string type.
 * @tparam T  std::string type. SFINAE will reject other ones.
 * @return The same string as the input argument.
 *
 * @details
 * This function makes the IP as a string from the input.
 */
template <typename T>
std::enable_if_t<std::is_same_v<T, std::string>, std::string> print_ip(const T &t)
{
    return t;
}

/**
 * @brief Print function specialization for std::vector and std::list types.
 * @tparam T  std::vector and std::list types. SFINAE will reject other ones.
 * @return The IP string composed from container items delimited by dots.
 *
 * @details
 * This function makes the IP as a string from the input.
 */
template <typename T>
std::enable_if_t<is_container<T>::value, std::string> print_ip(const T &t)
{
    std::ostringstream ret;
    const auto size = t.size();
    size_t i = 0;
    for (const auto &c : t)
    {
        ret << std::to_string(c);
        if (i < size - 1)
        {
            ret << ".";
        }
        ++i;
    }
    return ret.str();
}

template <typename T>
constexpr bool same_type(const T &)
{
    return true;
}

template <typename T, typename U, typename... Tp>
constexpr bool same_type(const std::tuple<T, U, Tp...> &)
{
    return std::is_same_v<T, U> && same_type(std::tuple<U, Tp...>{});
}

/**
 * @brief Closing print function specialization for std::tuple types.
 * @tparam T  std::tuple type. SFINAE will reject other ones.
 * @return The IP string composed from tuple items delimited by dots.
 *
 * @details
 * This function makes the IP as a string from the input. 
 * The input tuple is check its content if it contain the same type.
 */
template <size_t I = 0, typename... Tp>
inline typename std::enable_if_t<I == sizeof...(Tp) - 1, std::string> print_ip(const std::tuple<Tp...> &t)
{
    assert(same_type(t)); /**Only same types are allowed*/
    return std::to_string(std::get<I>(t));
}

/**
 * @brief Print function specialization for std::tuple types.
 * @tparam T  std::tuple type. SFINAE will reject other ones.
 * @return The IP string composed from tuple items delimited by dots.
 *
 * @details
 * This function makes the IP as a string from the input. 
 * The input tuple is check its content if it contain the same type.
 */
template <size_t I = 0, typename... Tp>
    inline typename std::enable_if_t < I<sizeof...(Tp) - 1, std::string> print_ip(const std::tuple<Tp...> &t)
{
    return std::to_string(std::get<I>(t)) + std::string(".") + print_ip<I + 1, Tp...>(t);
}