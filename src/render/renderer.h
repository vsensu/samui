#ifndef SAMUI_RENDERER_H_
#define SAMUI_RENDERER_H_

#include <glm/glm.hpp>

#include "core/core.h"
#include "core/shader.h"
#include "orthographic_camera.h"
#include "render_command.h"

namespace samui {
class SAMUI_API Renderer {
 public:
  static void BeginScene(OrthographicCamera& camera);
  static void EndScene();
  static void Submit(const std::shared_ptr<Shader>&      shader,
                     const std::shared_ptr<VertexArray>& vertex_array);

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

 private:
  struct SceneData {
    glm::mat4 view_proj_matrix;
  };

  static SceneData* scene_data_;
};
}  // namespace samui

#endif