#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// TODO: use C++20 concepts
template <typename T>
struct is_container : false_type
{
};

template <typename U>
struct is_container<std::vector<U>> : true_type
{
};

template <typename U>
struct is_container<std::list<U>> : true_type
{
};


template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::string> print_ip(const T &t)
{
    ostringstream ret;
    for (int i = sizeof(T) - 1; i >= 0; --i)
    {
        unsigned char c = t >> i * 8;
        ret << to_string(c);
        if (i > 0)
        {
            ret << ".";
        }
    }
    return ret.str();
}

template <typename T>
std::enable_if_t<std::is_same_v<T, std::string>, std::string> print_ip(const T &t)
{
    return t;
}

template <typename T>
std::enable_if_t<is_container<T>::value, std::string> print_ip(const T &t)
{
    ostringstream ret;
    const auto size = t.size();
    size_t i = 0;
    for (const auto &c : t)
    {
        ret << to_string(c);
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
inline typename std::enable_if_t<I == sizeof...(Tp) - 1, std::string> print_ip(const std::tuple<Tp...> &t)
{
    static_assert(same_type(t)); // Only same types are allowed
    return to_string(std::get<I>(t));
}

template <size_t I = 0, typename... Tp>
    inline typename std::enable_if_t < I<sizeof...(Tp) - 1, std::string> print_ip(const std::tuple<Tp...> &t)
{
    return to_string(std::get<I>(t)) + "."s + print_ip<I + 1, Tp...>(t);
}

int main(int argc, char const *argv[])
{
    try
    {
        cout << print_ip(char{-1}) << endl;
        cout << print_ip(short{0}) << endl;
        cout << print_ip(int{2130706433}) << endl;
        cout << print_ip(long{8875824491850138409}) << endl;
        cout << print_ip(std::string{"12.34.56.78"}) << endl;
        cout << print_ip(std::vector{1, 2, 1, 2}) << endl;
        cout << print_ip(std::list{3, 4, 5, 6, 7, 8}) << endl;
        cout << print_ip(std::make_tuple(12, 23, 34)) << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}