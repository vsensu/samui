// clang-format off
#include "renderer2d.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <log/log.h>
#include <profiler/instrumentor.h>
#include <rendering/shader.h>
#include <rendering/buffer.h>

#include "render_command.h"
#include "buffer_create.h"
#include "texture_create.h"
#include "shader_create.h"
// clang-format on

namespace samui
{
struct QuadVertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texcoord;
    uint32_t  texture_index;
    float     tiling_factor;

    // #ifdef SAMUI_EDITOR
    uint32_t entity_id{0};
    // #endif
};

struct Rendderer2DData
{
    constexpr static uint32_t max_quads = 20000;
    constexpr static uint32_t max_vertices = max_quads * 4;
    constexpr static uint32_t max_indices = max_quads * 6;
    constexpr static uint32_t max_texture_slots = 32;

    std::shared_ptr<VertexArray>  quad_vertex_array;
    std::shared_ptr<VertexBuffer> quad_vertex_buffer;
    std::shared_ptr<Shader>       texture_shader;
    std::shared_ptr<Texture2D>    white_texture;

    uint32_t    quad_index_count;
    QuadVertex* quad_vertex_buffer_base = nullptr;
    QuadVertex* quad_vertex_buffer_ptr = nullptr;

    std::array<std::shared_ptr<Texture2D>, max_texture_slots> texture_slots;
    uint32_t texture_slot_index = 1;  // 0 is white texture

    glm::vec4 quad_vertex_positions[4];

    Renderer2D::Statistics stats;
};

static Rendderer2DData renderer2d_data;

void Renderer2D::init()
{
    SAMUI_PROFILE_FUNCTION();
    renderer2d_data.quad_vertex_array = samui::vertex_array::create();
    renderer2d_data.quad_vertex_buffer = samui::vertex_buffer::create(
        Rendderer2DData::max_vertices * sizeof(QuadVertex));
    renderer2d_data.quad_vertex_buffer->set_layout({
        {"Position", ShaderDataType::Float3},
        {"Color", ShaderDataType::Float4},
        {"TexCoord", ShaderDataType::Float2},
        {"TexIndex", ShaderDataType::UInt},
        {"TilingFactor", ShaderDataType::Float},
        {"EntityID", ShaderDataType::Int},
    });
    renderer2d_data.quad_vertex_array->add_vertex_buffer(
        renderer2d_data.quad_vertex_buffer);

    renderer2d_data.quad_vertex_buffer_base =
        new QuadVertex[Rendderer2DData::max_vertices];

    uint32_t* quad_indices = new uint32_t[Rendderer2DData::max_indices];
    uint32_t  offset = 0;
    for (uint32_t i = 0; i < Rendderer2DData::max_indices; i += 6)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;
        offset += 4;
    }
    std::shared_ptr<IndexBuffer> quad_index_buffer =
        samui::index_buffer::create(quad_indices, Rendderer2DData::max_indices);
    renderer2d_data.quad_vertex_array->set_index_buffer(quad_index_buffer);
    delete[] quad_indices;

    renderer2d_data.white_texture = samui::texture2d::create(1, 1);
    uint32_t white_texture_data = 0xFFFFFFFF;
    renderer2d_data.white_texture->set_data(&white_texture_data,
                                           sizeof(white_texture_data));

    int32_t samplers[renderer2d_data.max_texture_slots];
    for (uint32_t i = 0; i < renderer2d_data.max_texture_slots; ++i)
    {
        samplers[i] = i;
    }

    renderer2d_data.texture_shader =
        samui::shader::create("engine/assets/shaders/2d.glsl");

    renderer2d_data.texture_shader->bind();
    renderer2d_data.texture_shader->set_int_array(
        "u_textures", samplers, renderer2d_data.max_texture_slots);

    renderer2d_data.texture_slots[0] = renderer2d_data.white_texture;

    renderer2d_data.quad_vertex_positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    renderer2d_data.quad_vertex_positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    renderer2d_data.quad_vertex_positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    renderer2d_data.quad_vertex_positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::shutdown() { SAMUI_PROFILE_FUNCTION(); }

void Renderer2D::begin_scene(const glm::mat4& view_proj)
{
    renderer2d_data.texture_shader->bind();
    renderer2d_data.texture_shader->set_mat4("viewProj", view_proj);
    renderer2d_data.quad_vertex_buffer_ptr =
        renderer2d_data.quad_vertex_buffer_base;

    renderer2d_data.quad_index_count = 0;
    renderer2d_data.texture_slot_index = 1;
}

void Renderer2D::begin_scene(const glm::mat4& projection,
                             const glm::mat4& transform)
{
    renderer2d_data.texture_shader->bind();
    renderer2d_data.texture_shader->set_mat4(
        "viewProj", projection * glm::inverse(transform));
    renderer2d_data.quad_vertex_buffer_ptr =
        renderer2d_data.quad_vertex_buffer_base;

    renderer2d_data.quad_index_count = 0;
    renderer2d_data.texture_slot_index = 1;
}

void Renderer2D::begin_scene(const OrthographicCamera& camera)
{
    SAMUI_PROFILE_FUNCTION();
    renderer2d_data.texture_shader->bind();
    renderer2d_data.texture_shader->set_mat4(
        "viewProj", camera.get_view_projection_matrix());
    renderer2d_data.quad_vertex_buffer_ptr =
        renderer2d_data.quad_vertex_buffer_base;

    renderer2d_data.quad_index_count = 0;
    renderer2d_data.texture_slot_index = 1;
}

void Renderer2D::end_scene()
{
    SAMUI_PROFILE_FUNCTION();

    uint32_t data_size = (uint8_t*)renderer2d_data.quad_vertex_buffer_ptr -
                         (uint8_t*)renderer2d_data.quad_vertex_buffer_base;
    renderer2d_data.quad_vertex_buffer->set_data(
        renderer2d_data.quad_vertex_buffer_base, data_size);

    flush();
}

void Renderer2D::flush()
{
    SAMUI_PROFILE_FUNCTION();
    if (renderer2d_data.quad_index_count == 0)
    {
        return;
    }

    for (uint32_t i = 0; i < renderer2d_data.texture_slot_index; ++i)
    {
        renderer2d_data.texture_slots[i]->bind(i);
    }
    RenderCommand::draw_indexed(renderer2d_data.quad_vertex_array,
                                renderer2d_data.quad_index_count);

    ++renderer2d_data.stats.draw_calls;
}

void Renderer2D::draw_quad(const glm::vec2& pos, const glm::vec2& size,
                           const glm::vec4& color)
{
    SAMUI_PROFILE_FUNCTION();
    draw_quad({pos.x, pos.y, 0.f}, size, color);
}

void Renderer2D::draw_quad(const glm::vec3& pos, const glm::vec2& size,
                           const glm::vec4& color)
{
    SAMUI_PROFILE_FUNCTION();
    auto transform =
        glm::translate(glm::identity<glm::mat4>(), pos) *
        glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
    draw_quad(transform, color);
}

void Renderer2D::draw_quad(const glm::vec2& pos, const glm::vec2& size,
                           const std::shared_ptr<Texture2D>& texture,
                           float tilingFactor, glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    draw_quad({pos.x, pos.y, 0.f}, size, texture, tilingFactor, tint);
}

void Renderer2D::draw_quad(const glm::vec3& pos, const glm::vec2& size,
                           const std::shared_ptr<Texture2D>& texture,
                           float tilingFactor, glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    auto transform =
        glm::translate(glm::identity<glm::mat4>(), pos) *
        glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
    draw_quad(transform, texture, tilingFactor, tint);
}

void Renderer2D::draw_quad(const glm::vec2& pos, const glm::vec2& size,
                           const std::shared_ptr<SubTexture2D>& subtexture,
                           float tilingFactor, glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    draw_quad({pos.x, pos.y, 0.f}, size, subtexture, tilingFactor, tint);
}

void Renderer2D::draw_quad(const glm::vec3& pos, const glm::vec2& size,
                           const std::shared_ptr<SubTexture2D>& subtexture,
                           float tilingFactor, glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    auto transform =
        glm::translate(glm::identity<glm::mat4>(), pos) *
        glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
    draw_quad(transform, subtexture, tilingFactor, tint);
}

void Renderer2D::draw_rotated_quad(const glm::vec2& pos, const glm::vec2& size,
                                   float rotation, const glm::vec4& color)
{
    SAMUI_PROFILE_FUNCTION();
    draw_rotated_quad({pos.x, pos.y, 0.f}, size, rotation, color);
}

void Renderer2D::draw_rotated_quad(const glm::vec3& pos, const glm::vec2& size,
                                   float rotation, const glm::vec4& color)
{
    SAMUI_PROFILE_FUNCTION();
    auto transform =
        glm::translate(glm::identity<glm::mat4>(), pos) *
        glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
        glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});

    draw_quad(transform, color);
}

void Renderer2D::draw_rotated_quad(const glm::vec2& pos, const glm::vec2& size,
                                   float                             rotation,
                                   const std::shared_ptr<Texture2D>& texture,
                                   float tilingFactor, glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    draw_rotated_quad({pos.x, pos.y, 0.f}, size, rotation, texture,
                      tilingFactor, tint);
}

void Renderer2D::draw_rotated_quad(const glm::vec3& pos, const glm::vec2& size,
                                   float                             rotation,
                                   const std::shared_ptr<Texture2D>& texture,
                                   float tilingFactor, glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    auto transform =
        glm::translate(glm::identity<glm::mat4>(), pos) *
        glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
        glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
    draw_quad(transform, texture, tilingFactor, tint);
}

void Renderer2D::draw_rotated_quad(
    const glm::vec2& pos, const glm::vec2& size, float rotation,
    const std::shared_ptr<SubTexture2D>& subtexture, float tilingFactor,
    glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    draw_rotated_quad({pos.x, pos.y, 0.f}, size, rotation, subtexture,
                      tilingFactor, tint);
}

void Renderer2D::draw_rotated_quad(
    const glm::vec3& pos, const glm::vec2& size, float rotation,
    const std::shared_ptr<SubTexture2D>& subtexture, float tilingFactor,
    glm::vec4 tint)
{
    SAMUI_PROFILE_FUNCTION();
    auto transform =
        glm::translate(glm::identity<glm::mat4>(), pos) *
        glm::rotate(glm::identity<glm::mat4>(), rotation, {0.f, 0.f, 1.f}) *
        glm::scale(glm::identity<glm::mat4>(), {size.x, size.y, 1.f});
    draw_quad(transform, subtexture, tilingFactor, tint);
}

void Renderer2D::draw_quad(const glm::mat4& transform, const glm::vec4& color,
                           entity_t entity_id)
{
    SAMUI_PROFILE_FUNCTION();
    draw_quad(transform,
              static_cast<const std::shared_ptr<Texture2D>&>(nullptr), 1.f,
              color, entity_id);
}
void Renderer2D::draw_quad(const glm::mat4&                  transform,
                           const std::shared_ptr<Texture2D>& texture,
                           float tilingFactor, glm::vec4 tint,
                           entity_t entity_id)
{
    SAMUI_PROFILE_FUNCTION();

    if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices)
    {
        flush_and_reset();
    }

    uint32_t texture_index = 0;
    if (texture != nullptr)
    {
        for (int i = 1; i < renderer2d_data.texture_slot_index; ++i)
        {
            if (*renderer2d_data.texture_slots[i].get() == *texture.get())
            {
                texture_index = i;
                break;
            }
        }

        if (texture_index == 0)
        {
            texture_index = renderer2d_data.texture_slot_index;
            renderer2d_data.texture_slots[renderer2d_data.texture_slot_index] =
                texture;
            ++renderer2d_data.texture_slot_index;
        }
    }

    constexpr glm::vec2 tex_coords[4] = {
        {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}};

    for (int i = 0; i < 4; ++i)
    {
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
void Renderer2D::draw_quad(const glm::mat4&                     transform,
                           const std::shared_ptr<SubTexture2D>& subtexture,
                           float tilingFactor, glm::vec4 tint,
                           entity_t entity_id)
{
    SAMUI_PROFILE_FUNCTION();

    if (renderer2d_data.quad_index_count >= Rendderer2DData::max_indices)
    {
        flush_and_reset();
    }

    uint32_t    texture_index = 0;
    const auto& texture = subtexture->get_texture();
    if (texture != nullptr)
    {
        for (int i = 1; i < renderer2d_data.texture_slot_index; ++i)
        {
            if (*renderer2d_data.texture_slots[i].get() == *texture.get())
            {
                texture_index = i;
                break;
            }
        }

        if (texture_index == 0)
        {
            texture_index = renderer2d_data.texture_slot_index;
            renderer2d_data.texture_slots[renderer2d_data.texture_slot_index] =
                texture;
            ++renderer2d_data.texture_slot_index;
        }
    }

    const auto& tex_coords = subtexture->get_tex_coords();

    for (int i = 0; i < 4; ++i)
    {
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

// void Renderer2D::draw_sprite(const glm::mat4&               transform,
//                              const SpriteRendererComponent& sprite,
//                              entity_t                   entity_id)
// {
//     SAMUI_PROFILE_FUNCTION();
//     if (sprite.texture != nullptr)
//     {
//         draw_quad(transform, sprite.texture, sprite.tiling_factor, sprite.color,
//                   entity_id);
//     }
//     else
//     {
//         draw_quad(transform, sprite.color, entity_id);
//     }
// }

void Renderer2D::reset_stats()
{
    memset(&renderer2d_data.stats, 0, sizeof(renderer2d_data.stats));
}

Renderer2D::Statistics Renderer2D::get_stats() { return renderer2d_data.stats; }

void Renderer2D::flush_and_reset()
{
    SAMUI_PROFILE_FUNCTION();
    end_scene();

    renderer2d_data.quad_vertex_buffer_ptr =
        renderer2d_data.quad_vertex_buffer_base;

    renderer2d_data.quad_index_count = 0;
    renderer2d_data.texture_slot_index = 1;
}

}  // namespace samui
