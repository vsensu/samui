#ifndef SAMUI_COMPONENTS_H_
#define SAMUI_COMPONENTS_H_

// clang-format off
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/core.h>
#include <rendering/texture.h>
// clang-format on

namespace samui {
void foo_components();

struct SAMUI_API NameComponent {
  std::string name;
};

struct SAMUI_API TransformComponent {
  glm::vec3 translation{glm::zero<glm::vec3>()};
  glm::vec3 rotation{glm::zero<glm::vec3>()};
  glm::vec3 scale{glm::vec3(1.f)};

  glm::mat4 transform() const {
    auto rot = glm::mat4_cast(glm::quat(rotation));

    return glm::translate(glm::identity<glm::mat4>(), translation) * rot *
           glm::scale(glm::identity<glm::mat4>(), scale);
  }
};

struct SAMUI_API SpriteRendererComponent {
  glm::vec4      color{1.0f, 1.0f, 1.0f, 1.0f};
  std::shared_ptr<Texture2D> texture{0};
  float          tiling_factor{1.0f};
};

struct SAMUI_API CameraComponent {
  enum class ProjectionType { Orthographic = 0, Perspective = 1 };

  ProjectionType projection_type = ProjectionType::Orthographic;
  float          aspect_ratio{1.0f};
  bool           fixed_aspect_ratio{false};

  float ortho_size{10.f};
  float ortho_near{-1.f};
  float ortho_far{1.f};

  float pers_fov{45.f};
  float pers_near{0.01f};
  float pers_far{1000.f};

  glm::mat4 projection{1.f};
};

namespace CameraUtils {
glm::mat4 SAMUI_API get_orthographic_projection(float aspect_ratio, float size,
                                                float z_near, float z_far);

glm::mat4 SAMUI_API get_perspective_projection(float aspect_ratio, float fov,
                                               float z_near, float z_far);
}  // namespace CameraUtils

}  // namespace samui

#endif  // SAMUI_COMPONENTS_H_