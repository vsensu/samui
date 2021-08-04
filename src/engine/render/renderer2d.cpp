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
  constexpr static uint32_t max_quads = 20000;
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

  glm::vec4 quad_vertex_positions[4];

  Renderer2D::Statistics stats;
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
      Shader::Create("engine/assets/shaders/2d.glsl");

  renderer2d_data.texture_shader->Bind();
  renderer2d_data.texture_shader->SetIntArray(
      "u_textures", samplers, renderer2d_data.max_texture_slots);

  renderer2d_data.texture_slots[0] = renderer2d_data.white_texture;

  renderer2d_data.quad_vertex_positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  renderer2d_data.quad_vertex_positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  renderer2d_data.quad_vertex_positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  renderer2d_data.quad_vertex_positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
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

  ++renderer2d_data.stats.draw_calls;
}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  DrawQuad({pos.x, pos.y, 0.f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

  constexpr float white_texture_index = 0.f;
  constexpr float tilingFactor = 1.f;

  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[0];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[1];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[2];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[3];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

  ++renderer2d_data.stats.quad_count;
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

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

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

  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[0];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[1];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[2];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[3];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

  ++renderer2d_data.stats.quad_count;
}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const Ref<SubTexture2D>& subtexture,
                          float tilingFactor, glm::vec4 tint) {
  DrawQuad({pos.x, pos.y, 0.f}, size, subtexture, tilingFactor, tint);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const Ref<SubTexture2D>& subtexture,
                          float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

  float       texture_index = 0.f;
  const auto& texture = subtexture->GetTexture();
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

  const auto* texture_coords = subtexture->GetTexCoords();

  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[0];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = texture_coords[0];
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[1];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = texture_coords[1];
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[2];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = texture_coords[2];
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[3];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = texture_coords[3];
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

  ++renderer2d_data.stats.quad_count;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                                 float rotation, const glm::vec4& color) {
  DrawRotatedQuad({pos.x, pos.y, 0.f}, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                                 float rotation, const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

  constexpr float white_texture_index = 0.f;
  constexpr float tilingFactor = 1.f;

  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[0];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[1];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[2];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[3];
  renderer2d_data.quad_vertex_buffer_ptr->color = color;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = white_texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

  ++renderer2d_data.stats.quad_count;
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

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

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

  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[0];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[1];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[2];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[3];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

  ++renderer2d_data.stats.quad_count;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                                 float                    rotation,
                                 const Ref<SubTexture2D>& subtexture,
                                 float tilingFactor, glm::vec4 tint) {
  DrawRotatedQuad({pos.x, pos.y, 0.f}, size, rotation, subtexture, tilingFactor,
                  tint);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                                 float                    rotation,
                                 const Ref<SubTexture2D>& subtexture,
                                 float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

  float       texture_index = 0.f;
  const auto& texture = subtexture->GetTexture();
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

  const auto* texture_coords = subtexture->GetTexCoords();

  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[0];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[1];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 0.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[2];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {1.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_vertex_buffer_ptr->position =
      transform * renderer2d_data.quad_vertex_positions[3];
  renderer2d_data.quad_vertex_buffer_ptr->color = tint;
  renderer2d_data.quad_vertex_buffer_ptr->texcoord = {0.f, 1.f};
  renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
  renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
  ++renderer2d_data.quad_vertex_buffer_ptr;

  renderer2d_data.quad_index_count += 6;

  ++renderer2d_data.stats.quad_count;
}

void Renderer2D::ResetStats() {
  memset(&renderer2d_data.stats, 0, sizeof(renderer2d_data.stats));
}

Renderer2D::Statistics Renderer2D::GetStats() { return renderer2d_data.stats; }

void Renderer2D::FlushAndReset() {
  SAMUI_PROFILE_FUNCTION();
  EndScene();

  renderer2d_data.quad_vertex_buffer_ptr =
      renderer2d_data.quad_vertex_buffer_base;

  renderer2d_data.quad_index_count = 0;
  renderer2d_data.texture_slot_index = 1;
}

}  // namespace samui