#ifndef SAMUI_RENDERER_H_
#define SAMUI_RENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/core.h"
#include "core/shader.h"
#include "orthographic_camera.h"
#include "render_command.h"

namespace samui {
class SAMUI_API Renderer {
 public:
  static void Init();
  static void BeginScene(const OrthographicCamera& camera);
  static void EndScene();
  static void Submit(const samui::Ref<Shader>&      shader,
                     const samui::Ref<VertexArray>& vertex_array,
                     const glm::mat4& transform = glm::identity<glm::mat4>());

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

 private:
  struct SceneData {
    glm::mat4 view_proj_matrix;
  };

  static SceneData* scene_data_;
};
}  // namespace samui

#endif