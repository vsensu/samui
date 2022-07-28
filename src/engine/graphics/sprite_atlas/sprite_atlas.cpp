#include "sprite_atlas.h"

namespace samui
{
SpriteAtlas::SpriteAtlas(std::shared_ptr<samui::Texture2D> sprite_sheet)
    : sprite_sheet_(sprite_sheet)
{
}

SpriteAtlas::SpriteAtlas(const std::filesystem::path& path)
    : sprite_sheet_(samui::texture2d::create(path))
{
}

void SpriteAtlas::add_sprite(sprite_id_t id, const glm::vec2& uv,
                             const glm::vec2& cell_size,
                             const glm::vec2& sprite_size)
{
    sprites_[id] = samui::SubTexture2D::create_from_coords(
        sprite_sheet_, uv, cell_size, sprite_size);
}

std::shared_ptr<samui::SubTexture2D> SpriteAtlas::get_sprite(
    sprite_id_t id) const
{
    auto find_result = sprites_.find(id);
    if (find_result != sprites_.end())
    {
        return find_result->second;
    }
    return nullptr;
}
}  // namespace samui
