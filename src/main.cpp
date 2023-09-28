#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <vector>
#ifdef CONCEPTS
#include <concepts.h>
#else
#include <sfinae.h>
#endif

using namespace std;

// TODO: use C++20 concepts

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
        cout << print_ip(std::make_tuple(12.0, 23, 34)) << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}