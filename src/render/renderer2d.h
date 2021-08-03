#ifndef SAMUI_RENDERER2D_H_
#define SAMUI_RENDERER2D_H_

#include "core/core.h"
#include "orthographic_camera.h"
#include "subtexture2d.h"
#include "texture.h"

namespace samui {

class SAMUI_API Renderer2D {
 public:
  static void Init();
  static void Shutdown();

  static void BeginScene(const OrthographicCamera& camera);
  static void EndScene();
  static void Flush();

  static void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                       const glm::vec4& color);
  static void DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                       const glm::vec4& color);

  static void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                       const Ref<Texture2D>& texture, float tilingFactor = 1.f,
                       glm::vec4 tint = glm::vec4(1.f));
  static void DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                       const Ref<Texture2D>& texture, float tilingFactor = 1.f,
                       glm::vec4 tint = glm::vec4(1.f));
  static void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                       const Ref<SubTexture2D>& subtexture,
                       float                    tilingFactor = 1.f,
                       glm::vec4                tint = glm::vec4(1.f));
  static void DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                       const Ref<SubTexture2D>& subtexture,
                       float                    tilingFactor = 1.f,
                       glm::vec4                tint = glm::vec4(1.f));

  static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                              float rotation, const glm::vec4& color);
  static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                              float rotation, const glm::vec4& color);

  static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                              float rotation, const Ref<Texture2D>& texture,
                              float     tilingFactor = 1.f,
                              glm::vec4 tint = glm::vec4(1.f));
  static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                              float rotation, const Ref<Texture2D>& texture,
                              float     tilingFactor = 1.f,
                              glm::vec4 tint = glm::vec4(1.f));
  static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
                              float rotation, const Ref<SubTexture2D>& subtexture,
                              float     tilingFactor = 1.f,
                              glm::vec4 tint = glm::vec4(1.f));
  static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
                              float rotation, const Ref<SubTexture2D>& subtexture,
                              float     tilingFactor = 1.f,
                              glm::vec4 tint = glm::vec4(1.f));

  struct Statistics {
    uint32_t draw_calls = 0;
    uint32_t quad_count = 0;

    uint32_t get_total_vertex_count() const { return quad_count * 4; }
  };

  static void       ResetStats();
  static Statistics GetStats();

 private:
  static void FlushAndReset();
};

}  // namespace samui

#endif  // SAMUI_RENDERER2D_H_