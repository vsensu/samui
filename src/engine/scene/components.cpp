#include "components.h"

#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace samui {
void foo_components() {
  entt::registry registry;
  registry.view<NameComponent>();
  registry.view<TransformComponent>();
  registry.view<SpriteRendererComponent>();
  registry.view<CameraComponent>();
}

namespace CameraUtils {
glm::mat4 get_projection(float aspect_ratio, float size, float z_near,
                         float z_far) {
  float left = -size * aspect_ratio * 0.5f;
  float right = size * aspect_ratio * 0.5f;
  float bottom = -size * 0.5f;
  float top = size * 0.5f;
  return glm::ortho(left, right, bottom, top);
}
}  // namespace CameraUtils

}  // namespace samui
