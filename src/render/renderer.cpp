#include "renderer.h"

namespace samui {
Renderer::SceneData* Renderer::scene_data_ = new Renderer::SceneData;

void Renderer::Init() { RenderCommand::Init(); }

void Renderer::BeginScene(OrthographicCamera& camera) {
  scene_data_->view_proj_matrix = camera.get_view_projection_matrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const samui::Ref<Shader>&      shader,
                      const samui::Ref<VertexArray>& vertex_array,
                      const glm::mat4&               transform) {
  shader->Bind();
  shader->UploadUniform("viewProj", scene_data_->view_proj_matrix);
  shader->UploadUniform("transform", transform);

  vertex_array->Bind();
  RenderCommand::DrawIndexed(vertex_array);
}
}  // namespace samui
