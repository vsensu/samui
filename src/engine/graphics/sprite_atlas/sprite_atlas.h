#pragma once

// clang-format off
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <string>

#include <rendering/subtexture2d.h>

#include "../renderer/texture_create.h"

#include "../graphics_module.h"
// clang-format on

namespace samui
{

using sprite_id_t = std::string;
class SAMUI_GRAPHICS_API SpriteAtlas
{
public:
    explicit SpriteAtlas(std::shared_ptr<samui::Texture2D> sprite_sheet);

    explicit SpriteAtlas(const std::filesystem::path& path);

    void add_sprite(sprite_id_t id, const glm::vec2& uv,
                    const glm::vec2& cell_size,
                    const glm::vec2& sprite_size = {1, 1});

    std::shared_ptr<samui::SubTexture2D> get_sprite(sprite_id_t id) const;
    std::shared_ptr<samui::Texture2D> sprite_sheet() const { return sprite_sheet_; }

private:
    std::shared_ptr<samui::Texture2D> sprite_sheet_;
    std::unordered_map<sprite_id_t, std::shared_ptr<samui::SubTexture2D>>
        sprites_;
};

}  // namespace samui