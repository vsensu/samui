#pragma once

// clang-format off
#include <unordered_map>
#include <filesystem>
#include <memory>

#include <rendering/subtexture2d.h>

#include "../graphics_module.h"
#include "../renderer/texture_create.h"
// clang-format on

namespace samui
{

class SAMUI_GRAPHICS_API SpriteAtlas
{
public:
    using sprite_id_t = uint32_t;
    explicit SpriteAtlas(std::shared_ptr<samui::Texture2D> sprite_sheet);

    explicit SpriteAtlas(const std::filesystem::path& path);

    void add_sprite(sprite_id_t id, const glm::vec2& uv,
                    const glm::vec2& cell_size,
                    const glm::vec2& sprite_size = {1, 1});

    std::shared_ptr<samui::SubTexture2D> get_sprite(sprite_id_t id) const;

private:
    std::shared_ptr<samui::Texture2D> sprite_sheet_;
    std::unordered_map<sprite_id_t, std::shared_ptr<samui::SubTexture2D>>
        sprites_;
};

}  // namespace samui