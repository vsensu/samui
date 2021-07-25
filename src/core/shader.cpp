#include "shader.h"

#include "../render/renderer.h"
#include "opengl_shader.h"

namespace samui {

Shader* Shader::Create(const std::string& filepath) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(filepath);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

Shader* Shader::Create(const std::string& vertex_shader,
                       const std::string& fragment_shader) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(vertex_shader, fragment_shader);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}
}  // namespace samui
