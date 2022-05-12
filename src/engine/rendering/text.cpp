// clang-format off
#include "text.h"

#include <log/log.h>
// clang-format on

namespace samui
{
void Text::init()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        SAMUI_ENGINE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");

    FT_Face face;
    if (FT_New_Face(ft, "engine/assets/fonts/OpenSans/OpenSans-Regular.ttf", 0,
                    &face))
        SAMUI_ENGINE_ERROR("ERROR::FREETYPE: Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, 48);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        SAMUI_ENGINE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
}

}  // namespace samui