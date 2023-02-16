#pragma once

// clang-format off
#include <catch2/catch.hpp>

#include <engine/serialization/serialization.h>
// clang-format on

struct CustomType
{
    int value;
};

// custom type need to specialization YAML::convert<T>
namespace YAML
{
template <>
struct convert<CustomType>
{
    static Node encode(const CustomType& rhs)
    {
        Node node;
        node.push_back(rhs.value);
        return node;
    }
};
}

TEST_CASE("serialize 4 is equal to 4 (success)", "[tag1]")
{
    REQUIRE(samui::serialization::serialize(4) == "4");
}

TEST_CASE("serialize CustomType is equal to '0' (fail)", "[tag1]")
{
    REQUIRE(samui::serialization::serialize(CustomType()) != "0");
}
