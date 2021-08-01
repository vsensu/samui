#include "renderer2d.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/shader.h"
#include "buffer.h"
#include "debug/instrumentor.h"
#include "render_command.h"

namespace samui {
struct QuadVertex {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 texcoord;
  float     texture_index;
  float     tiling_factor;
};

struct Rendderer2DData {
  constexpr static uint32_t max_quads = 10000;
  constexpr static uint32_t max_vertices = max_quads * 4;
  constexpr static uint32_t max_indices = max_quads * 6;
  constexpr static uint32_t max_texture_slots = 32;

  Ref<VertexArray>  quad_vertex_array;
  Ref<VertexBuffer> quad_vertex_buffer;
  Ref<Shader>       texture_shader;
  Ref<Texture2D>    white_texture;

  uint32_t    quad_index_count;
  QuadVertex* quad_vertex_buffer_base = nullptr;
  QuadVertex* quad_vertex_buffer_ptr = nullptr;

  std::array<Ref<Texture2D>, max_texture_slots> texture_slots;
  uint32_t texture_slot_index = 1;  // 0 is white texture
};

static Rendderer2DData renderer2d_data;

void Renderer2D::Init() {
  SAMUI_PROFILE_FUNCTION();
  renderer2d_data.quad_vertex_array = VertexArray::Create();
  renderer2d_data.quad_vertex_buffer =
      VertexBuffer::Create(Rendderer2DData::max_vertices * sizeof(QuadVertex));
  renderer2d_data.quad_vertex_buffer->SetLayout({
      {"Position", ShaderDataType::Float3},
      {"Color", ShaderDataType::Float4},
      {"TexCoord", ShaderDataType::Float2},
      {"TexIndex", ShaderDataType::Float},
      {"TilingFactor", ShaderDataType::Float},
  });
  renderer2d_data.quad_vertex_array->AddVertexBuffer(
      renderer2d_data.quad_vertex_buffer);

  renderer2d_data.quad_vertex_buffer_base =
      new QuadVertex[Rendderer2DData::max_vertices];

  uint32_t* quad_indices = new uint32_t[Rendderer2DData::max_indices];
  uint32_t  offset = 0;
  for (uint32_t i = 0; i < Rendderer2DData::max_indices; i += 6) {
    quad_indices[i + 0] = offset + 0;
    quad_indices[i + 1] = offset + 1;
    quad_indices[i + 2] = offset + 2;

    quad_indices[i + 3] = offset + 2;
    quad_indices[i + 4] = offset + 3;
    quad_indices[i + 5] = offset + 0;
    offset += 4;
  }
  Ref<IndexBuffer> quad_index_buffer =
      IndexBuffer::Create(quad_indices, Rendderer2DData::max_indices);
  renderer2d_data.quad_vertex_array->SetIndexBuffer(quad_index_buffer);
  delete[] quad_indices;

  renderer2d_data.white_texture = Texture2D::Create(1, 1);
  uint32_t white_texture_data = 0xFFFFFFFF;
  renderer2d_data.white_texture->SetData(&white_texture_data,
                                         sizeof(white_texture_data));

  int32_t samplers[renderer2d_data.max_texture_slots];
  for (uint32_t i = 0; i < renderer2d_data.max_texture_slots; ++i) {
    samplers[i] = i;
  }

  renderer2d_data.texture_shader =
      Shader::Create("assets/shaders/texture.glsl");

  renderer2d_data.texture_shader->Bind();
  renderer2d_data.texture_shader->SetIntArray(
      "u_textures", samplers, renderer2d_data.max_texture_slots);

  renderer2d_data.texture_slots[0] = renderer2d_data.white_texture;
}

void Renderer2D::Shutdown() { SAMUI_PROFILE_FUNCTION(); }

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
  SAMUI_PROFILE_FUNCTION();
  renderer2d_data.texture_shader->Bind();
  renderer2d_data.texture_shader->SetMat4("viewProj",
                                          camera.get_view_projection_matrix());
  renderer2d_data.quad_vertex_buffer_ptr =
      renderer2d_data.quad_vertex_buffer_base;

  renderer2d_data.quad_index_count = 0;
  renderer2d_data.texture_slot_index = 1;
}

void Renderer2D::EndScene() {
  SAMUI_PROFILE_FUNCTION();

  uint32_t data_size = (uint8_t*)renderer2d_data.quad_vertex_buffer_ptr -
                       (uint8_t*)renderer2d_data.quad_vertex_buffer_base;
  renderer2d_data.quad_vertex_buffer->SetData(
      renderer2d_data.quad_vertex_buffer_base, data_size);

  Flush();
}

void Renderer2D::Flush() {
  SAMUI_PROFILE_FUNCTION();
  for (uint32_t i = 0; i < renderer2d_data.texture_slot_index; ++i) {
    renderer2d_data.texture_slots[i]->Bind(i);
  }
  RenderCommand::DrawIndexed(renderer2d_data.quad_vertex_array,
                             renderer2d_data.quad_index_count);
}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  DrawQuad({pos.x, pos.y, 0.f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();

  constexpr float white_texture_index = 0.f;
  constexpr float tilingFactor = 1.f;

  renderer2d_data.quad_vertex_buffer_ptr->position = pos;
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position = {pos.x + size.x, pos.y,
                                                      0.f};
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position = {pos.x + size.x,
                                                      pos.y + size.y, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position = {pos.x, pos.y + size.y,
                                                      0.f};
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

  /*
    renderer2d_data.white_texture->Bind();
    renderer2d_data.texture_shader->SetFloat("u_tiling_factor", 1.f);
    renderer2d_data.texture_shader->SetFloat4("u_color", color);
    auto transform =
        glm::translate(glm::identity<glm::mat4>(), pos) *
        glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
    renderer2d_data.texture_shader->SetMat4("transform", transform);

    renderer2d_data.quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(renderer2d_data.quad_vertex_array);
    */
}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          glm::vec4 tint) {
  DrawQuad({pos.x, pos.y, 0.f}, size, texture, tilingFactor, tint);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();

  constexpr glm::vec4 color = {1.f, 1.f, 1.f, 1.f};

  float texture_index = 0.f;

  for (int i = 1; i < renderer2d_data.texture_slot_index; ++i) {
    if (*renderer2d_data.texture_slots[i].get() == *texture.get()) {
      texture_index = i;
      break;
    }
  }

  if (texture_index == 0.f) {
    texture_index = (float)renderer2d_data.texture_slot_index;
    renderer2d_data.texture_slots[renderer2d_data.texture_slot_index] = texture;
    ++renderer2d_data.texture_slot_index;
  }

  renderer2d_data.quad_vertex_buffer_ptr->position = pos;
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position = {pos.x + size.x, pos.y,
                                                      0.f};
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position = {pos.x + size.x,
                                                      pos.y + size.y, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position = {pos.x, pos.y + size.y,
                                                      0.f};
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

#if OLD_PATH
  texture->Bind();
  renderer2d_data.texture_shader->SetFloat("u_tiling_factor", tilingFactor);
  renderer2d_data.texture_shader->SetFloat4("u_color", tint);
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  renderer2d_data.texture_shader->SetMat4("transform", transform);

  renderer2d_data.quad_vertex_array->Bind();
  RenderCommand::DrawIndexed(renderer2d_data.quad_vertex_array);
#endif
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                                 float rotation, const glm::vec4& color) {
  DrawRotatedQuad({pos.x, pos.y, 0.f}, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                                 float rotation, const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();
  renderer2d_data.white_texture->Bind();
  renderer2d_data.texture_shader->SetFloat("u_tiling_factor", 1.f);
  renderer2d_data.texture_shader->SetFloat4("u_color", color);
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  renderer2d_data.texture_shader->SetMat4("transform", transform);

  renderer2d_data.quad_vertex_array->Bind();
  RenderCommand::DrawIndexed(renderer2d_data.quad_vertex_array);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                                 float rotation, const Ref<Texture2D>& texture,
                                 float tilingFactor, glm::vec4 tint) {
  DrawRotatedQuad({pos.x, pos.y, 0.f}, size, rotation, texture, tilingFactor,
                  tint);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                                 float rotation, const Ref<Texture2D>& texture,
                                 float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  texture->Bind();
  renderer2d_data.texture_shader->SetFloat("u_tiling_factor", tilingFactor);
  renderer2d_data.texture_shader->SetFloat4("u_color", tint);
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  renderer2d_data.texture_shader->SetMat4("transform", transform);

  renderer2d_data.quad_vertex_array->Bind();
  RenderCommand::DrawIndexed(renderer2d_data.quad_vertex_array);
}

}  // namespace samui
