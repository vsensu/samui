#ifndef SAMUI_SUBTEXTURE2D_H_
#define SAMUI_SUBTEXTURE2D_H_

// clang-format off
#include <glm/glm.hpp>

#include <core/core.h>
#include "texture.h"
// clang-format on

namespace samui {
class SAMUI_API SubTexture2D {
 public:
  SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min,
               const glm::vec2& max);

  const Ref<Texture2D>& GetTexture() const { return texture_; }

  const glm::vec2* GetTexCoords() const { return tex_coords_; }

  static Ref<SubTexture2D> CreateFromCoords(
      const Ref<Texture2D>& texture, const glm::vec2& coords,
      const glm::vec2& cellSize, const glm::vec2& spriteSize = {1, 1});

 private:
  Ref<Texture2D> texture_;
  glm::vec2      tex_coords_[4];
};
}  // namespace samui

#endif  // SAMUI_SUBTEXTURE2D_H_