#include "renderer2d.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/shader.h"
#include "buffer.h"
#include "debug/instrumentor.h"
#include "render_command.h"

namespace samui {
struct Rendderer2DStorage {
  Ref<VertexArray> vertex_array;
  Ref<Shader>      texture_shader;
  Ref<Texture2D>   white_texture;
};

static Rendderer2DStorage* renderer2d_storage;

void Renderer2D::Init() {
  SAMUI_PROFILE_FUNCTION();
  renderer2d_storage = new Rendderer2DStorage();
  renderer2d_storage->vertex_array = VertexArray::Create();

  // clang-format off
    float square_vertices[5 * 4] = {
      -0.5f, -0.5f, 0.f, 0.f, 0.f,
      0.5f, -0.5f, 0.f, 1.f, 0.f,
      0.5f, 0.5f, 0.f, 1.f, 1.f,
      -0.5f, 0.5f, 0.f, 0.f, 1.f
    };
  // clang-format on
  Ref<VertexBuffer> square_vertex_buffer;
  square_vertex_buffer =
      VertexBuffer::Create(square_vertices, sizeof(square_vertices));
  BufferLayout square_layout = {
      {"Position", ShaderDataType::Float3},
      {"TexCoord", ShaderDataType::Float2},
  };
  square_vertex_buffer->SetLayout(square_layout);
  renderer2d_storage->vertex_array->AddVertexBuffer(square_vertex_buffer);
  uint32_t         square_indices[6] = {0, 1, 2, 2, 3, 0};
  Ref<IndexBuffer> square_index_buffer;
  square_index_buffer = IndexBuffer::Create(
      square_indices, sizeof(square_indices) / sizeof(uint32_t));
  renderer2d_storage->vertex_array->SetIndexBuffer(square_index_buffer);

  renderer2d_storage->white_texture = Texture2D::Create(1, 1);
  uint32_t white_texture_data = 0xFFFFFFFF;
  renderer2d_storage->white_texture->SetData(&white_texture_data,
                                             sizeof(white_texture_data));

  renderer2d_storage->texture_shader =
      Shader::Create("assets/shaders/texture.glsl");

  renderer2d_storage->texture_shader->Bind();
  renderer2d_storage->texture_shader->SetInt("u_texture", 0);
}

void Renderer2D::Shutdown() {
  SAMUI_PROFILE_FUNCTION();
  delete renderer2d_storage;
  renderer2d_storage = nullptr;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
  SAMUI_PROFILE_FUNCTION();
  renderer2d_storage->texture_shader->Bind();
  renderer2d_storage->texture_shader->SetMat4(
      "viewProj", camera.get_view_projection_matrix());
}

void Renderer2D::EndScene() { SAMUI_PROFILE_FUNCTION(); }

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  DrawQuad({pos.x, pos.y, 0.f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();
  renderer2d_storage->white_texture->Bind();

  renderer2d_storage->texture_shader->SetFloat4("u_color", color);
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  renderer2d_storage->texture_shader->SetMat4("transform", transform);

  renderer2d_storage->vertex_array->Bind();
  RenderCommand::DrawIndexed(renderer2d_storage->vertex_array);
}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const Ref<Texture2D>& texture) {
  DrawQuad({pos.x, pos.y, 0.f}, size, texture);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const Ref<Texture2D>& texture) {
  SAMUI_PROFILE_FUNCTION();
  texture->Bind();
  renderer2d_storage->texture_shader->SetFloat4("u_color",
                                                glm::one<glm::vec4>());
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  renderer2d_storage->texture_shader->SetMat4("transform", transform);

  renderer2d_storage->vertex_array->Bind();
  RenderCommand::DrawIndexed(renderer2d_storage->vertex_array);
}

}  // namespace samui
