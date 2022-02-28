#include "renderer.h"

#include "renderer2d.h"
#include "debug/instrumentor.h"

namespace samui {
Renderer::SceneData* Renderer::scene_data_ = new Renderer::SceneData;

void Renderer::Init() {
  SAMUI_PROFILE_FUNCTION();

  RenderCommand::Init();
  Renderer2D::Init();
}

void Renderer::Shutdown() {
  SAMUI_PROFILE_FUNCTION();

  Renderer2D::Shutdown();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(const OrthographicCamera& camera) {
  scene_data_->view_proj_matrix = camera.get_view_projection_matrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>&      shader,
                      const std::shared_ptr<VertexArray>& vertex_array,
                      const glm::mat4&               transform) {
  shader->Bind();
  shader->SetMat4("viewProj", scene_data_->view_proj_matrix);
  shader->SetMat4("transform", transform);

  vertex_array->Bind();
  RenderCommand::DrawIndexed(vertex_array);
}
}  // namespace samui
