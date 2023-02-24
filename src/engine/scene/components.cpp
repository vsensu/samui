// clang-format off
#include "components.h"

#include <entt/entt.hpp>
// clang-format on

namespace samui
{
void foo_components()
{
    entt::registry registry;
    registry.view<NameComponent>();
    registry.view<TransformComponent>();
    registry.view<SpriteRendererComponent>();
    registry.view<CameraComponent>();
}

namespace CameraUtils
{
glm::mat4 get_orthographic_projection(float aspect_ratio, float size,
                                                float z_near, float z_far)
{
    float left = -size * aspect_ratio * 0.5f;
    float right = size * aspect_ratio * 0.5f;
    float bottom = -size * 0.5f;
    float top = size * 0.5f;
    return glm::ortho(left, right, bottom, top, z_near, z_far);
}

glm::mat4 get_perspective_projection(float aspect_ratio, float fov,
                                               float z_near, float z_far)
{
    return glm::perspective(glm::radians(fov), aspect_ratio, z_near, z_far);
}

}  // namespace CameraUtils

}  // namespace samui
