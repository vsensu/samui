#pragma once

#include <catch2/catch.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

TEST_CASE("ERROR::FREETYPE: Could not init FreeType Library", "[freetype]") {
  FT_Library ft;
  REQUIRE(FT_Init_FreeType(&ft) == 0);
}
