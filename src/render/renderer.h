#ifndef SAMUI_RENDERER_H_
#define SAMUI_RENDERER_H_

#include "core/core.h"
#include "orthographic_camera.h"
#include "render_command.h"

namespace samui {
class SAMUI_API Renderer {
 public:
  static void BeginScene(OrthographicCamera& camera);
  static void EndScene();
  static void Submit(const std::shared_ptr<VertexArray>& va /*todo: , shader*/);

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

 private:
  struct SceneData {
  };
};
}  // namespace samui

#endif