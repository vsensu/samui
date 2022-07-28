#pragma once

// ref: https://www.youtube.com/watch?v=UdwdJWQ5o78

consteval auto as_consteval(auto value)
{
    return value;
}

template<typename ... Param>
consteval decltype(auto) consteval_invoke(Param && ... param)
{
    return std::invoke(std::forward<Param>(param)...);
}
