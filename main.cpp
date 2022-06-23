/**
 * @file print_ip.cpp
 * @author Mikhail Samsonov
 * @brief methods for printing ip_address
 * @version 0.1
 * @date 2021-08-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

const char delimiter = '.';

/**
 * @brief print integral typename ip address
 * 
 * @tparam T
 * @param value
 * @return std::enable_if_t<std::is_integral_v<T>, void> 
 */
template <typename T>
std::enable_if_t<std::is_integral_v<T>, void> print_ip(T value)
{
    size_t typename_size = sizeof(T);
    for (size_t i = typename_size; i--;)
    {
        std::cout << (value >> (i * 8) & 0xFF);
        if (i > 0)
        {
            std::cout << delimiter;
        }
    }
    std::cout << std::endl;
}

/**
 * @brief print string typename ip address
 * 
 * @param value
 */
void print_ip(const std::string& value)
{
    std::cout << value << std::endl;
}

/**
 * @brief class to identify vector or list
 * 
 * @tparam T
 */
template <typename T>
struct is_vector_or_list : std::false_type
{
};

template <typename T>
struct is_vector_or_list<std::vector<T>> : std::true_type
{
};

template <typename T>
struct is_vector_or_list<std::list<T>> : std::true_type
{
};

/**
 * @brief print vector or list typename ip address
 * 
 * @tparam T 
 * @param container 
 * @return std::enable_if_t<is_vector_or_list<T>::value, void> 
 */
template <typename T>
std::enable_if_t<is_vector_or_list<T>::value, void> print_ip(const T& container)
{
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        std::cout << *it;
        if (it != std::prev(container.end()))
        {
            std::cout << delimiter;
        };
    }
    std::cout << std::endl;
}

/**
 * @brief 
 * 
 * @tparam I parameter pack size
 */
template <size_t I>
struct ip_in_tuple
{
    template <typename Head, typename... Tail>
    static void print(const std::tuple<Head, Tail...>& tuple)
    {
        auto element = std::get<sizeof...(Tail) + 1 - I>(tuple);
        static_assert(std::is_same_v<Head, decltype(element)>, "different types in tuple!");

        std::cout << element;
        if (I > 1)
        {
            std::cout << delimiter;
        }

        ip_in_tuple<I - 1>::print(tuple);
    }
};

template <>
struct ip_in_tuple<0>
{
    template <typename... Args>
    static void print(const std::tuple<Args...>&) {}
};

/**
 * @brief 
 * 
 * @tparam Types 
 * @param tuple 
 */
template <typename... Args>
void print_ip(const std::tuple<Args...>& tuple)
{
    ip_in_tuple<sizeof...(Args)>::print(tuple);
    std::cout << std::endl;
}

/**
 * @brief main function
 * 
 * @return int 
 */
int main()
{
    try
    {
        char ip_char{ -1 };
        print_ip(ip_char);

        short ip_short{ 0 };
        print_ip(ip_short);

        int ip_int{ 2130706433 };
        print_ip(ip_int);

        long ip_long{ 8875824491850138409 };
        print_ip(ip_long);

        print_ip("123.45.67.89.12.34.56");

        using ip_list = std::list<int>;
        ip_list l{ 255, 254, 253, 252 };
        print_ip<ip_list>(l);

        using ip_vector = std::vector<std::string>;
        ip_vector v{ "aaa", "bbb", "ccc", "ddd", "eee", "fff" };
        print_ip<ip_vector>(v);

        using ip_tuple_int = std::tuple<int, int, int, int>;
        ip_tuple_int t_int{ 1, 2, 3, 4 };
        print_ip(t_int);

        using ip_tuple_str = std::tuple<std::string, std::string, std::string, std::string>;
        ip_tuple_str t_str{ "abc", "def", "ghi", "gkl" };
        print_ip(t_str);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}