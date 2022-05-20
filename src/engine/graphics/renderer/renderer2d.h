#pragma once

// clang-format off
#include <memory>

#include <rendering/orthographic_camera.h>
#include <rendering/subtexture2d.h>
#include <rendering/texture.h>
// #include <scene/components.h>

#include "renderer_module.h"
// #include <entt/entt.hpp>
using entity_t = uint32_t;
// clang-format on

namespace samui
{

class SAMUI_RENDERER_API Renderer2D
{
  public:
    static void init();
    static void shutdown();

    static void begin_scene(const glm::mat4& view_proj);
    static void begin_scene(const glm::mat4& projection,
                            const glm::mat4& transform);
    static void begin_scene(const OrthographicCamera& camera);
    static void end_scene();
    static void flush();

    static void draw_quad(const glm::vec2& pos, const glm::vec2& size,
                          const glm::vec4& color);
    static void draw_quad(const glm::vec3& pos, const glm::vec2& size,
                          const glm::vec4& color);

    static void draw_quad(const glm::vec2& pos, const glm::vec2& size,
                          const std::shared_ptr<Texture2D>& texture,
                          float                             tilingFactor = 1.f,
                          glm::vec4 tint = glm::vec4(1.f));
    static void draw_quad(const glm::vec3& pos, const glm::vec2& size,
                          const std::shared_ptr<Texture2D>& texture,
                          float                             tilingFactor = 1.f,
                          glm::vec4 tint = glm::vec4(1.f));
    static void draw_quad(const glm::vec2& pos, const glm::vec2& size,
                          const std::shared_ptr<SubTexture2D>& subtexture,
                          float     tilingFactor = 1.f,
                          glm::vec4 tint = glm::vec4(1.f));
    static void draw_quad(const glm::vec3& pos, const glm::vec2& size,
                          const std::shared_ptr<SubTexture2D>& subtexture,
                          float     tilingFactor = 1.f,
                          glm::vec4 tint = glm::vec4(1.f));

    static void draw_rotated_quad(const glm::vec2& pos, const glm::vec2& size,
                                  float rotation, const glm::vec4& color);
    static void draw_rotated_quad(const glm::vec3& pos, const glm::vec2& size,
                                  float rotation, const glm::vec4& color);

    static void draw_rotated_quad(const glm::vec2& pos, const glm::vec2& size,
                                  float                             rotation,
                                  const std::shared_ptr<Texture2D>& texture,
                                  float     tilingFactor = 1.f,
                                  glm::vec4 tint = glm::vec4(1.f));
    static void draw_rotated_quad(const glm::vec3& pos, const glm::vec2& size,
                                  float                             rotation,
                                  const std::shared_ptr<Texture2D>& texture,
                                  float     tilingFactor = 1.f,
                                  glm::vec4 tint = glm::vec4(1.f));
    static void draw_rotated_quad(
        const glm::vec2& pos, const glm::vec2& size, float rotation,
        const std::shared_ptr<SubTexture2D>& subtexture,
        float tilingFactor = 1.f, glm::vec4 tint = glm::vec4(1.f));
    static void draw_rotated_quad(
        const glm::vec3& pos, const glm::vec2& size, float rotation,
        const std::shared_ptr<SubTexture2D>& subtexture,
        float tilingFactor = 1.f, glm::vec4 tint = glm::vec4(1.f));

    static void draw_quad(const glm::mat4& transform, const glm::vec4& color,
                          entity_t entity_id = 0);
    static void draw_quad(const glm::mat4&                  transform,
                          const std::shared_ptr<Texture2D>& texture,
                          float                             tilingFactor = 1.f,
                          glm::vec4    tint = glm::vec4(1.f),
                          entity_t entity_id = 0);
    static void draw_quad(const glm::mat4&                     transform,
                          const std::shared_ptr<SubTexture2D>& subtexture,
                          float        tilingFactor = 1.f,
                          glm::vec4    tint = glm::vec4(1.f),
                          entity_t entity_id = 0);

    // static void draw_sprite(const glm::mat4&               transform,
                            // const SpriteRendererComponent& sprite,
                            // entity_t entity_id = 0);

    struct Statistics
    {
        uint32_t draw_calls = 0;
        uint32_t quad_count = 0;

        uint32_t get_total_vertex_count() const { return quad_count * 4; }
    };

    static void       reset_stats();
    static Statistics get_stats();

  private:
    static void flush_and_reset();
};

}  // namespace samui
