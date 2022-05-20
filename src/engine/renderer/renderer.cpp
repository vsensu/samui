// clang-format off
#include "renderer.h"

#include <log/log.h>
#include <profiler/instrumentor.h>

#include "renderer2d.h"
// clang-format on

namespace samui
{
Renderer::SceneData* Renderer::scene_data_ = new Renderer::SceneData;

void Renderer::init()
{
    SAMUI_PROFILE_FUNCTION();

    RenderCommand::init();
    Renderer2D::init();
}

void Renderer::shutdown()
{
    SAMUI_PROFILE_FUNCTION();

    Renderer2D::shutdown();
}

void Renderer::on_window_resize(uint32_t width, uint32_t height)
{
    RenderCommand::set_viewport(0, 0, width, height);
}

void Renderer::begin_scene(const OrthographicCamera& camera)
{
    scene_data_->view_proj_matrix = camera.get_view_projection_matrix();
}

void Renderer::end_scene() {}

void Renderer::submit(const std::shared_ptr<Shader>&      shader,
                      const std::shared_ptr<VertexArray>& vertex_array,
                      const glm::mat4&                    transform)
{
    shader->bind();
    shader->set_mat4("viewProj", scene_data_->view_proj_matrix);
    shader->set_mat4("transform", transform);

    vertex_array->bind();
    RenderCommand::draw_indexed(vertex_array);
}
}  // namespace samui
