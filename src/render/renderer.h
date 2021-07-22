#ifndef SAMUI_RENDERER_H_
#define SAMUI_RENDERER_H_

#include "core/core.h"
#include "render_command.h"

namespace samui {
class SAMUI_API Renderer {
 public:
  static void BeginScene();
  static void EndScene();
  static void Submit(const std::shared_ptr<VertexArray>& va);

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};
}  // namespace samui

#endif