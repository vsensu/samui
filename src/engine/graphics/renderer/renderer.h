#pragma once

// clang-format off
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rendering/shader.h>
#include <rendering/orthographic_camera.h>

#include "renderer_module.h"
#include "render_command.h"
// clang-format on

namespace samui
{
class SAMUI_RENDERER_API Renderer
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

    inline static RenderCommand::API get_api() { return RenderCommand::get_api(); }

  private:
    struct SceneData
    {
        glm::mat4 view_proj_matrix;
    };

    static SceneData* scene_data_;
};
}  // namespace samui
