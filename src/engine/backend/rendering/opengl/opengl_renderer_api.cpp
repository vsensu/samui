// clang-format off
#include "opengl_renderer_api.h"

#include <glad/glad.h>

#include <debug/instrumentor.h>
#include "gl_errors.h"
// clang-format on

namespace samui {
void OpenGLRendererAPI::Init() {
  SAMUI_PROFILE_FUNCTION();

  initGLDebug();

  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const samui::Ref<VertexArray>& vertex_array,
                                    uint32_t index_count) {
  uint32_t count = index_count == 0 ? vertex_array->GetIndexBuffer()->GetCount()
                                    : index_count;
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  // glBindTexture(GL_TEXTURE_2D, 0);
}
}  // namespace samui