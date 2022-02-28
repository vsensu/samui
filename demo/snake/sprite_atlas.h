#pragma once

// clang-format off
#include <unordered_map>
#include <filesystem>
#include <memory>

#include <samui.h>
// clang-format on

template <typename IDType>
class SpriteAtlas
{
  public:
    explicit SpriteAtlas(std::shared_ptr<samui::Texture2D> sprite_sheet)
        : sprite_sheet_(sprite_sheet)
    {
    }

    explicit SpriteAtlas(const std::filesystem::path& path)
        : sprite_sheet_(samui::Texture2D::Create(path))
    {
    }

    void add_sprite(IDType id, const glm::vec2& uv, const glm::vec2& cell_size,
                    const glm::vec2& sprite_size = {1, 1})
    {
        sprites_[id] = samui::SubTexture2D::CreateFromCoords(
            sprite_sheet_, uv, cell_size, sprite_size);
    }

    std::shared_ptr<samui::SubTexture2D> get_sprite(IDType id) const
    {
        auto find_result = sprites_.find(id);
        if (find_result != sprites_.end())
        {
            return find_result->second;
        }
        return nullptr;
    }

  private:
    std::shared_ptr<samui::Texture2D>                                sprite_sheet_;
    std::unordered_map<IDType, std::shared_ptr<samui::SubTexture2D>> sprites_;
};
