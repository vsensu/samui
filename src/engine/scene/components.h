#ifndef SAMUI_COMPONENTS_H_
#define SAMUI_COMPONENTS_H_

#include <glm/glm.hpp>

namespace samui {

struct TransformComponent {
  glm::mat4 transform{1.0f};
};

struct SpriteRendererComponent {
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
};

}  // namespace samui

#endif  // SAMUI_COMPONENTS_H_