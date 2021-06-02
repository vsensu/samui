#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

TEST_CASE("1 is equal to 1 (success)", "[tag1]") {
  REQUIRE(1 == 1);
}

TEST_CASE("1 is not equal to 0 (fail)", "[tag1]") {
  REQUIRE(1 == 0);
}
