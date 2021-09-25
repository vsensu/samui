#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

// TEST_CASE("1 is equal to 1 (success)", "[tag1]") {
//   REQUIRE(1 == 1);
// }

// TEST_CASE("1 is not equal to 0 (fail)", "[tag1]") {
//   REQUIRE(1 == 0);
// }

TEST_CASE("ERROR::FREETYPE: Could not init FreeType Library", "[freetype]") {
  FT_Library ft;
  REQUIRE(FT_Init_FreeType(&ft) == 0);
}
