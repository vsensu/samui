#include "subtexture2d.h"

namespace samui {
SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min,
                           const glm::vec2& max)
    : texture_(texture) {
  tex_coords_[0] = {min.x, min.y};
  tex_coords_[1] = {max.x, min.y};
  tex_coords_[2] = {max.x, max.y};
  tex_coords_[3] = {min.x, max.y};
}

std::shared_ptr<SubTexture2D> SubTexture2D::CreateFromCoords(const std::shared_ptr<Texture2D>& texture,
                                                 const glm::vec2&      coords,
                                                 const glm::vec2&      cellSize,
                                                 const glm::vec2& spriteSize) {
  glm::vec2 min = {coords.x * cellSize.x / texture->GetWidth(),
                   coords.y * cellSize.y / texture->GetHeight()};
  glm::vec2 max = {
      (coords.x + spriteSize.x) * cellSize.x / texture->GetWidth(),
      (coords.y + spriteSize.y) * cellSize.y / texture->GetHeight()};
  return std::make_shared<SubTexture2D>(texture, min, max);
}

}  // namespace samui
