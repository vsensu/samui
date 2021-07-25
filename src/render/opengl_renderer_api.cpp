#include "opengl_renderer_api.h"

#include <glad/glad.h>

namespace samui {
void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const samui::Ref<VertexArray>& va) {
  glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(),
                 GL_UNSIGNED_INT, nullptr);
}
}  // namespace samui