#include "shader.h"

#include "../render/renderer.h"
#include "opengl_shader.h"

namespace samui {
Shader* Shader::Create(const char* vertex_shader, const char* fragment_shader) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(vertex_shader, fragment_shader);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}
}  // namespace samui
