#include "renderer2d.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/shader.h"
#include "buffer.h"
#include "render_command.h"

namespace samui {
struct Rendderer2DStorage {
  Ref<VertexArray> vertex_array;
  Ref<Shader>      flat_color_shader;
};

static Rendderer2DStorage* renderer2d_storage;

void Renderer2D::Init() {
  renderer2d_storage = new Rendderer2DStorage();
  renderer2d_storage->vertex_array = VertexArray::Create();

  // clang-format off
    float square_vertices[3 * 4] = {
      -0.5f, -0.5f, 0.f,
      0.5f, -0.5f, 0.f,
      0.5f, 0.5f, 0.f,
      -0.5f, 0.5f, 0.f
    };
  // clang-format on
  Ref<VertexBuffer> square_vertex_buffer;
  square_vertex_buffer =
      VertexBuffer::Create(square_vertices, sizeof(square_vertices));
  BufferLayout square_layout = {
      {"Position", ShaderDataType::Float3},
  };
  square_vertex_buffer->SetLayout(square_layout);
  renderer2d_storage->vertex_array->AddVertexBuffer(square_vertex_buffer);
  uint32_t         square_indices[6] = {0, 1, 2, 2, 3, 0};
  Ref<IndexBuffer> square_index_buffer;
  square_index_buffer = IndexBuffer::Create(
      square_indices, sizeof(square_indices) / sizeof(uint32_t));
  renderer2d_storage->vertex_array->SetIndexBuffer(square_index_buffer);

  renderer2d_storage->flat_color_shader =
      Shader::Create("assets/shaders/flatcolor.glsl");
}

void Renderer2D::Shutdown() {
  delete renderer2d_storage;
  renderer2d_storage = nullptr;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
  renderer2d_storage->flat_color_shader->Bind();
  renderer2d_storage->flat_color_shader->UploadUniform(
      "viewProj", camera.get_view_projection_matrix());
  renderer2d_storage->flat_color_shader->UploadUniform(
      "transform", glm::identity<glm::mat4>());
}

void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  DrawQuad({pos.x, pos.y, 0.f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  renderer2d_storage->flat_color_shader->Bind();
  renderer2d_storage->flat_color_shader->UploadUniform("u_color", color);

  renderer2d_storage->vertex_array->Bind();
  RenderCommand::DrawIndexed(renderer2d_storage->vertex_array);
}

}  // namespace samui
