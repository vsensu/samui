#include "orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "debug/instrumentor.h"

namespace samui {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top)
    : projection_matrix_(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
      view_matrix_(1.0f) {
  SAMUI_PROFILE_FUNCTION();
  view_proj_matrix_ = projection_matrix_ * view_matrix_;
}

void OrthographicCamera::set_projection(float left, float right, float bottom,
                                        float top) {
  SAMUI_PROFILE_FUNCTION();
  projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  view_proj_matrix_ = projection_matrix_ * view_matrix_;
}

void OrthographicCamera::recalculate_view_matrix() {
  SAMUI_PROFILE_FUNCTION();
  glm::mat4 transform =
      glm::translate(glm::identity<glm::mat4>(), position_) *
      glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation_),
                  glm::vec3(0.0f, 0.0f, 1.0f));

  view_matrix_ = glm::inverse(transform);
  view_proj_matrix_ = projection_matrix_ * view_matrix_;
}

}  // namespace samui