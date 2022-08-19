#define CATCH_CONFIG_MAIN

// clang-format off
#include <catch2/catch.hpp>

#include <engine/serialization/serialization.h>
// clang-format on

struct CustomType
{
    int value;
};

namespace samui::serialization
{

template <>
std::string to_serialize<int>(const int& value)
{
    return std::to_string(value);
}

template <>
std::string to_serialize<CustomType>(const CustomType& value)
{
    return "HelloCustom";
}

}  // namespace samui::serialization

TEST_CASE("serialize 4 is equal to 4 (success)", "[tag1]")
{
    REQUIRE(samui::serialization::to_serialize(4) == "4");
}

TEST_CASE("serialize CustomType is equal to 'HelloCustom' (success)", "[tag1]")
{
    REQUIRE(samui::serialization::to_serialize(CustomType()) == "HelloCustom");
}