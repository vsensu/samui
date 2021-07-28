#ifndef SAMUI_RENDERER2D_H_
#define SAMUI_RENDERER2D_H_

#include "core/core.h"
#include "orthographic_camera.h"
#include "texture.h"

namespace samui {

class SAMUI_API Renderer2D {
 public:
  static void Init();
  static void Shutdown();

  static void BeginScene(const OrthographicCamera& camera);
  static void EndScene();

  static void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                       const glm::vec4& color);
  static void DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                       const glm::vec4& color);

  static void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                       const Ref<Texture2D>& texture);
  static void DrawQuad(const glm::vec3& pos, const glm::vec2& size,
                       const Ref<Texture2D>& texture);
};

}  // namespace samui

#endif  // SAMUI_RENDERER2D_H_