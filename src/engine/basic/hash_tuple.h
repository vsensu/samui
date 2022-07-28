#pragma once

#include <tuple>

class HashTuple
{
    template <class T>
    struct component
    {
        const T& value;
        component(const T& value) : value(value) {}
        uintmax_t operator,(uintmax_t n) const
        {
            n ^= std::hash<T>()(value);
            n ^= n << (sizeof(uintmax_t) * 4 - 1);
            return n ^ std::hash<uintmax_t>()(n);
        }
    };

   public:
    template <class Tuple>
    size_t operator()(const Tuple& tuple) const
    {
        return std::hash<uintmax_t>()(std::apply(
            [](const auto&... xs) { return (component(xs), ..., 0); }, tuple));
    }
};
