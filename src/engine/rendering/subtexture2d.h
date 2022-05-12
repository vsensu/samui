#pragma once

// clang-format off
#include <memory>

#include <glm/glm.hpp>

#include <core/core.h>
#include "texture.h"
// clang-format on

namespace samui
{
class SAMUI_API SubTexture2D
{
  public:
    SubTexture2D(const std::shared_ptr<Texture2D>& texture,
                 const glm::vec2& min, const glm::vec2& max);

    const std::shared_ptr<Texture2D>& get_texture() const { return texture_; }

    const glm::vec2* get_tex_coords() const { return tex_coords_; }

    static std::shared_ptr<SubTexture2D> create_from_coords(
        const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords,
        const glm::vec2& cellSize, const glm::vec2& spriteSize = {1, 1});

  private:
    std::shared_ptr<Texture2D> texture_;
    glm::vec2                  tex_coords_[4];
};
}  // namespace samui
