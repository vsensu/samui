#ifndef SAMUI_RENDER_COMMAND_H_
#define SAMUI_RENDER_COMMAND_H_

#include "renderer_api.h"

namespace samui {
class SAMUI_API RenderCommand {
 public:
  inline static void Init() { renderer_api_->Init(); }

  inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) {
    renderer_api_->SetViewport(x, y, width, height);
  }

  inline static void SetClearColor(const glm::vec4& color) {
    renderer_api_->SetClearColor(color);
  }
  inline static void Clear() { renderer_api_->Clear(); }

  inline static void DrawIndexed(const samui::Ref<VertexArray>& vertex_array, uint32_t index_count = 0) {
    renderer_api_->DrawIndexed(vertex_array,index_count);
  }

 private:
  static RendererAPI* renderer_api_;
};
}  // namespace samui

#endif  // SAMUI_RENDER_COMMAND_H_