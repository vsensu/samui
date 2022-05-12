#pragma once

// clang-format off
#include <glm/glm.hpp>

#include "core/core.h"
// clang-format on

namespace samui
{
class SAMUI_API OrthographicCamera
{
  public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void set_projection(float left, float right, float bottom, float top);

    const glm::vec3& get_position() const { return position_; }
    float            get_rotation() const { return rotation_; }

    void set_position(const glm::vec3& position)
    {
        position_ = position;
        recalculate_view_matrix();
    }

    void set_rotation(float rotation)
    {
        rotation_ = rotation;
        recalculate_view_matrix();
    }

    const glm::mat4& get_projection_matrix() const
    {
        return projection_matrix_;
    }
    const glm::mat4& get_view_matrix() const { return view_matrix_; }
    const glm::mat4& get_view_projection_matrix() const
    {
        return view_proj_matrix_;
    }

  private:
    void recalculate_view_matrix();

  private:
    glm::mat4 projection_matrix_;
    glm::mat4 view_matrix_;
    glm::mat4 view_proj_matrix_;

    glm::vec3 position_{0.f, 0.f, 0.f};
    float     rotation_{0.f};
};

}  // namespace samui
