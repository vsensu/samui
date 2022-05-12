#pragma once

// clang-format off
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/core.h>
#include "shader.h"
#include "orthographic_camera.h"
#include "render_command.h"
// clang-format on

namespace samui
{
class SAMUI_API Renderer
{
  public:
    static void init();
    static void shutdown();
    static void on_window_resize(uint32_t width, uint32_t height);
    static void begin_scene(const OrthographicCamera& camera);
    static void end_scene();
    static void submit(const std::shared_ptr<Shader>&      shader,
                       const std::shared_ptr<VertexArray>& vertex_array,
                       const glm::mat4& transform = glm::identity<glm::mat4>());

    inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }

  private:
    struct SceneData
    {
        glm::mat4 view_proj_matrix;
    };

    static SceneData* scene_data_;
};
}  // namespace samui
