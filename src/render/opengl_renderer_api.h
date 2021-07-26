#ifndef SAMUI_OPENGL_RENDERER_API_H_
#define SAMUI_OPENGL_RENDERER_API_H_

#include "renderer_api.h"

namespace samui {
class SAMUI_API OpenGLRendererAPI : public RendererAPI {
 public:
  void Init() override;
  void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height) override;
  void SetClearColor(const glm::vec4& color) override;
  void Clear() override;
  void DrawIndexed(const samui::Ref<VertexArray>& va) override;
};
}  // namespace samui

#endif  // SAMUI_OPENGL_RENDERER_API_H_