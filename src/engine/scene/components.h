#ifndef SAMUI_COMPONENTS_H_
#define SAMUI_COMPONENTS_H_

// clang-format off
#include <glm/glm.hpp>

#include <core/core.h>
// clang-format on

namespace samui {

struct SAMUI_API TransformComponent {
  glm::mat4 transform{1.0f};
};

struct SAMUI_API SpriteRendererComponent {
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
};

}  // namespace samui

#endif  // SAMUI_COMPONENTS_H_