#pragma once

#include <iostream>

namespace atlas
{
// https://eli.thegreenplace.net/2014/variadic-templates-in-c/
template <template <typename, typename...> typename ContainerType, typename ValueType, typename... Args>
void printContainer(const ContainerType<ValueType, Args...>& container)
{
    for (const auto& value : container)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}

// Implement << for pairs: this is needed to print out mappings where range
// iteration goes over (key, value) pairs.
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p)
{
    out << '[' << p.first << ", " << p.second << ']';
    return out;
}
}
