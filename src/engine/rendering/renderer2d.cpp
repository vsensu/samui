// clang-format off
#include "renderer2d.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "buffer.h"
#include "render_command.h"
#include <debug/instrumentor.h>
// clang-format on

namespace samui {
struct QuadVertex {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 texcoord;
  float     texture_index;
  float     tiling_factor;

  // #ifdef SAMUI_EDITOR
  uint32_t entity_id{entt::null};
  // #endif
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
      {"EntityID", ShaderDataType::Int},
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

void Renderer2D::BeginScene(const glm::mat4& view_proj) {
  renderer2d_data.texture_shader->Bind();
  renderer2d_data.texture_shader->SetMat4("viewProj", view_proj);
  renderer2d_data.quad_vertex_buffer_ptr =
      renderer2d_data.quad_vertex_buffer_base;

  renderer2d_data.quad_index_count = 0;
  renderer2d_data.texture_slot_index = 1;
}

void Renderer2D::BeginScene(const glm::mat4& projection,
                            const glm::mat4& transform) {
  renderer2d_data.texture_shader->Bind();
  renderer2d_data.texture_shader->SetMat4("viewProj",
                                          projection * glm::inverse(transform));
  renderer2d_data.quad_vertex_buffer_ptr =
      renderer2d_data.quad_vertex_buffer_base;

  renderer2d_data.quad_index_count = 0;
  renderer2d_data.texture_slot_index = 1;
}

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
  SAMUI_PROFILE_FUNCTION();
  DrawQuad({pos.x, pos.y, 0.f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  DrawQuad({pos.x, pos.y, 0.f}, size, texture, tilingFactor, tint);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  DrawQuad(transform, texture, tilingFactor, tint);
}

void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                          const Ref<SubTexture2D>& subtexture,
                          float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  DrawQuad({pos.x, pos.y, 0.f}, size, subtexture, tilingFactor, tint);
}

void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                          const Ref<SubTexture2D>& subtexture,
                          float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  DrawQuad(transform, subtexture, tilingFactor, tint);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                                 float rotation, const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();
  DrawRotatedQuad({pos.x, pos.y, 0.f}, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                                 float rotation, const glm::vec4& color) {
  SAMUI_PROFILE_FUNCTION();
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

  DrawQuad(transform, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                                 float rotation, const Ref<Texture2D>& texture,
                                 float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  DrawRotatedQuad({pos.x, pos.y, 0.f}, size, rotation, texture, tilingFactor,
                  tint);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                                 float rotation, const Ref<Texture2D>& texture,
                                 float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  DrawQuad(transform, texture, tilingFactor, tint);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                                 float                    rotation,
                                 const Ref<SubTexture2D>& subtexture,
                                 float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  DrawRotatedQuad({pos.x, pos.y, 0.f}, size, rotation, subtexture, tilingFactor,
                  tint);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                                 float                    rotation,
                                 const Ref<SubTexture2D>& subtexture,
                                 float tilingFactor, glm::vec4 tint) {
  SAMUI_PROFILE_FUNCTION();
  auto transform =
      glm::translate(glm::identity<glm::mat4>(), pos) *
      glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
      glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
  DrawQuad(transform, subtexture, tilingFactor, tint);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                          entt::entity entity_id) {
  SAMUI_PROFILE_FUNCTION();
  DrawQuad(transform, static_cast<const Ref<Texture2D>&>(nullptr), 1.f, color,
           entity_id);
}
void Renderer2D::DrawQuad(const glm::mat4&      transform,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          glm::vec4 tint, entt::entity entity_id) {
  SAMUI_PROFILE_FUNCTION();

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

  float texture_index = 0.f;
  if (texture != nullptr) {
    for (int i = 1; i < renderer2d_data.texture_slot_index; ++i) {
      if (*renderer2d_data.texture_slots[i].get() == *texture.get()) {
        texture_index = i;
        break;
      }
    }

    if (texture_index == 0.f) {
      texture_index = (float)renderer2d_data.texture_slot_index;
      renderer2d_data.texture_slots[renderer2d_data.texture_slot_index] =
          texture;
      ++renderer2d_data.texture_slot_index;
    }
  }

  constexpr glm::vec2 tex_coords[4] = {
      {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}};

  for (int i = 0; i < 4; ++i) {
    renderer2d_data.quad_vertex_buffer_ptr->position =
        transform * renderer2d_data.quad_vertex_positions[i];
    renderer2d_data.quad_vertex_buffer_ptr->color = tint;
    renderer2d_data.quad_vertex_buffer_ptr->texcoord = tex_coords[i];
    renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
    renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
    renderer2d_data.quad_vertex_buffer_ptr->entity_id =
        static_cast<uint32_t>(entity_id);
    ++renderer2d_data.quad_vertex_buffer_ptr;
  }

  renderer2d_data.quad_index_count += 6;
  ++renderer2d_data.stats.quad_count;
}
void Renderer2D::DrawQuad(const glm::mat4&         transform,
                          const Ref<SubTexture2D>& subtexture,
                          float tilingFactor, glm::vec4 tint,
                          entt::entity entity_id) {
  SAMUI_PROFILE_FUNCTION();

  if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices) {
    FlushAndReset();
  }

  float       texture_index = 0.f;
  const auto& texture = subtexture->GetTexture();
  if (texture != nullptr) {
    for (int i = 1; i < renderer2d_data.texture_slot_index; ++i) {
      if (*renderer2d_data.texture_slots[i].get() == *texture.get()) {
        texture_index = i;
        break;
      }
    }

    if (texture_index == 0.f) {
      texture_index = (float)renderer2d_data.texture_slot_index;
      renderer2d_data.texture_slots[renderer2d_data.texture_slot_index] =
          texture;
      ++renderer2d_data.texture_slot_index;
    }
  }

  const auto& tex_coords = subtexture->GetTexCoords();

  for (int i = 0; i < 4; ++i) {
    renderer2d_data.quad_vertex_buffer_ptr->position =
        transform * renderer2d_data.quad_vertex_positions[i];
    renderer2d_data.quad_vertex_buffer_ptr->color = tint;
    renderer2d_data.quad_vertex_buffer_ptr->texcoord = tex_coords[i];
    renderer2d_data.quad_vertex_buffer_ptr->texture_index = texture_index;
    renderer2d_data.quad_vertex_buffer_ptr->tiling_factor = tilingFactor;
    renderer2d_data.quad_vertex_buffer_ptr->entity_id =
        static_cast<uint32_t>(entity_id);
    ++renderer2d_data.quad_vertex_buffer_ptr;
  }

  renderer2d_data.quad_index_count += 6;
  ++renderer2d_data.stats.quad_count;
}

void Renderer2D::DrawSprite(const glm::mat4&               transform,
                            const SpriteRendererComponent& sprite,
                            entt::entity                   entity_id) {
  SAMUI_PROFILE_FUNCTION();
  if (sprite.texture != nullptr) {
    DrawQuad(transform, sprite.texture, sprite.tiling_factor, sprite.color,
             entity_id);
  } else {
    DrawQuad(transform, sprite.color, entity_id);
  }
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
