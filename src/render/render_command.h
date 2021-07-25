#ifndef SAMUI_RENDER_COMMAND_H_
#define SAMUI_RENDER_COMMAND_H_

#include "renderer_api.h"

namespace samui {
class SAMUI_API RenderCommand {
 public:
  inline static void Init() { renderer_api_->Init(); }

  inline static void SetClearColor(const glm::vec4& color) {
    renderer_api_->SetClearColor(color);
  }
  inline static void Clear() { renderer_api_->Clear(); }

  inline static void DrawIndexed(const samui::Ref<VertexArray>& va) {
    renderer_api_->DrawIndexed(va);
  }

 private:
  static RendererAPI* renderer_api_;
};
}  // namespace samui

#endif  // SAMUI_RENDER_COMMAND_H_