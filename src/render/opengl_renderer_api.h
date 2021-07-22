#ifndef SAMUI_OPENGL_RENDERER_API_H_
#define SAMUI_OPENGL_RENDERER_API_H_

#include "renderer_api.h"

namespace samui {
class SAMUI_API OpenGLRendererAPI : public RendererAPI {
 public:
  void SetClearColor(const glm::vec4& color) override;
  void Clear() override;
  void DrawIndexed(const std::shared_ptr<VertexArray>& va) override;
};
}  // namespace samui

#endif  // SAMUI_OPENGL_RENDERER_API_H_