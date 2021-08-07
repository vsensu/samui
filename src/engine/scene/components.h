#ifndef SAMUI_COMPONENTS_H_
#define SAMUI_COMPONENTS_H_

// clang-format off
#include <string>

#include <glm/glm.hpp>

#include <core/core.h>
// clang-format on

namespace samui {
void foo_components();

struct SAMUI_API NameComponent {
  std::string name;
};

struct SAMUI_API TransformComponent {
  glm::mat4 transform{1.0f};
};

struct SAMUI_API SpriteRendererComponent {
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
};

struct SAMUI_API CameraComponent {
  float     aspect_ratio{1.0f};
  float     size{10.f};
  float     z_near{-1.f};
  float     z_far{1.f};
  glm::mat4 projection{1.f};
};

namespace CameraUtils {
glm::mat4 SAMUI_API get_projection(float aspect_ratio, float size, float z_near,
                         float z_far);
}  // namespace CameraUtils

}  // namespace samui

#endif  // SAMUI_COMPONENTS_H_