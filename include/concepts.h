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
* The concepts file shows abilities of concepts approach in C++20.
*/
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <type_traits>
#include <concepts>
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

template <typename T>
requires (std::is_integral_v<T>)
std::string print_ip(const T &t)
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

template <typename T>
requires (std::is_same_v<T, std::string>)
std::string print_ip(const T &t)
{
    return t;
}

template <typename T>
requires (is_container<T>::value)
std::string print_ip(const T &t)
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

template <size_t I = 0, typename... Tp>
inline constexpr typename std::enable_if_t<I == sizeof...(Tp) - 1, std::string> print_ip(const std::tuple<Tp...> &t)
{
    assert(same_type(t)); // Only same types are allowed
    return std::to_string(std::get<I>(t));
}

template <size_t I = 0, typename... Tp>
inline constexpr typename std::enable_if_t < I<sizeof...(Tp) - 1, std::string> print_ip(const std::tuple<Tp...> &t)
{
    return std::to_string(std::get<I>(t)) + std::string(".") + print_ip<I + 1, Tp...>(t);
}